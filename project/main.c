
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "iom361_r4.h"
#include "bits.h"
#include "status.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"

#define START_SWITCH 0x1u
#define DOOR_SWITCH  0x2u

#define INLET_LED  0x1u
#define HEATER_LED 0x2u
#define DRUM_LED   0x4u
#define PUMP_LED   0x8u

#define MAX_TICKS 100000

typedef struct {
    size_t tick;
    float temp;
    float humidity;
    unsigned switches;
} scenario_event_t;

typedef struct {
    scenario_event_t *events;
    size_t count;
    size_t capacity;
    time_t start;
} scenario_t;

static void scenario_free(scenario_t *s)
{
    free(s->events);
    s->events = NULL;
    s->count = 0;
    s->capacity = 0;
}

static int scenario_load(const char *filename, scenario_t *s)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror(filename);
        return -1;
    }

    char line[256];
    int have_start = 0;
    size_t previous_tick = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *comment = strchr(line, '#');

        if (comment != NULL)
            *comment = '\0';

        char *p = line;

        while (*p == ' ' || *p == '\t')
            p++;

        if (*p == '\0' || *p == '\n')
            continue;

        if (!have_start)
        {
            long long start;

            if (sscanf(p, "start %lld", &start) != 1)
                goto invalid;

            s->start = (time_t)start;

            if ((long long)s->start != start)
                goto invalid;

            have_start = 1;
            continue;
        }

        scenario_event_t event;
        unsigned long long tick;

        if (sscanf(p, "%llu %f %f %x",
                   &tick,
                   &event.temp,
                   &event.humidity,
                   &event.switches) != 4)
        {
            goto invalid;
        }

        if (tick > MAX_TICKS ||
            (s->count > 0 && tick <= previous_tick))
        {
            goto invalid;
        }

        event.tick = (size_t)tick;
        previous_tick = event.tick;

        if (s->count == s->capacity)
        {
            size_t capacity =
                s->capacity == 0 ? 8 : s->capacity * 2;

            scenario_event_t *new_events = realloc(
                s->events,
                capacity * sizeof(*new_events)
            );

            if (new_events == NULL)
                goto invalid;

            s->events = new_events;
            s->capacity = capacity;
        }

        s->events[s->count++] = event;
    }

    fclose(file);

    if (!have_start || s->count == 0)
        return -1;

    return 0;

invalid:
    fprintf(stderr, "Invalid scenario file.\n");
    fclose(file);
    return -1;
}

static size_t phase_duration(washer_state_t state)
{
    switch (state)
    {
        case STATE_FILL:  return 20;
        case STATE_WASH:  return 300;
        case STATE_RINSE: return 120;
        case STATE_SPIN:  return 60;
        case STATE_DRAIN: return 30;
        default:          return 0;
    }
}

static washer_event_t completion_event(washer_state_t state)
{
    switch (state)
    {
        case STATE_FILL:  return EVENT_FILL_COMPLETE;
        case STATE_WASH:  return EVENT_WASH_COMPLETE;
        case STATE_RINSE: return EVENT_RINSE_COMPLETE;
        case STATE_SPIN:  return EVENT_SPIN_COMPLETE;
        case STATE_DRAIN: return EVENT_DRAIN_COMPLETE;
        default:          return EVENT_COUNT;
    }
}

static uint32_t led_value(washer_state_t state,
                          float temperature,
                          bool *heater)
{
    uint32_t leds = 0;

    if (state != STATE_WASH)
        *heater = false;

    switch (state)
    {
        case STATE_FILL:
            leds = INLET_LED;
            break;

        case STATE_WASH:
            leds = DRUM_LED;

            if (temperature < 38.5f)
                *heater = true;
            else if (temperature >= 40.0f)
                *heater = false;

            if (*heater)
                leds |= HEATER_LED;
            break;

        case STATE_RINSE:
            leds = INLET_LED | DRUM_LED;
            break;

        case STATE_SPIN:
            leds = DRUM_LED;
            break;

        case STATE_DRAIN:
            leds = PUMP_LED;
            break;

        default:
            break;
    }

    return leds;
}

static uint32_t rgb_value(washer_state_t state)
{
    switch (state)
    {
        case STATE_IDLE:
            return build_rgb_reg(1, 0, 255, 0);
        case STATE_FILL:
            return build_rgb_reg(1, 0, 0, 255);
        case STATE_WASH:
            return build_rgb_reg(1, 255, 0, 0);
        case STATE_RINSE:
            return build_rgb_reg(1, 0, 255, 255);
        case STATE_SPIN:
            return build_rgb_reg(1, 255, 0, 255);
        case STATE_DRAIN:
            return build_rgb_reg(1, 255, 255, 0);
        default:
            return 0;
    }
}

static void print_trace(time_t timestamp,
                        washer_state_t state,
                        reading_t reading,
                        uint32_t leds,
                        uint32_t status)
{
    struct tm *utc = gmtime(&timestamp);
    char date[32];

    if (utc == NULL ||
        strftime(date, sizeof(date),
                 "%Y-%m-%d %H:%M:%S", utc) == 0)
    {
        return;
    }

    printf("%s state=%s temp=%.1f rh=%.1f "
           "leds=0x%X status=0x%08X\n",
           date,
           fsm_state_name(state),
           reading.temp_c,
           reading.rh_pct,
           (unsigned)leds,
           (unsigned)status);
}

static void print_history(aux_t *history)
{
    aux_event_t event;

    printf("Event history (newest first):\n");

    while (aux_pop(history, &event) == 0)
    {
        printf("tick=%zu state=%s\n",
               event.tick,
               fsm_state_name(event.state));
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3 ||
        (argc == 3 && strcmp(argv[2], "--report") != 0))
    {
        fprintf(stderr,
                "Usage: %s scenario.txt [--report]\n",
                argv[0]);
        return 1;
    }

    bool report_only = argc == 3;
    scenario_t scenario = {0};

    if (scenario_load(argv[1], &scenario) != 0)
    {
        scenario_free(&scenario);
        return 1;
    }

    int rc = 0;
    uint32_t *base = iom361_initialize(2, 4, &rc);

    if (base == NULL || rc != 0)
    {
        fprintf(stderr, "Hardware initialization failed.\n");
        scenario_free(&scenario);
        return 1;
    }

    iom361_set_display(false);

    store_t *store = store_create();
    aux_t *history = aux_create();
    report_t *report = report_create(3);

    if (store == NULL || history == NULL || report == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        store_destroy(store);
        aux_destroy(history);
        report_destroy(report);
        scenario_free(&scenario);
        return 1;
    }

    washer_state_t state = STATE_IDLE;
    size_t phase_start = 0;
    size_t next_event = 0;
    size_t last_input =
        scenario.events[scenario.count - 1].tick;

    float temperature = 23.5f;
    float humidity = 75.0f;
    unsigned switches = 0;
    unsigned previous_switches = 0;

    bool heater = false;
    bool started = false;
    bool finished = false;
    int failed = 0;

    for (size_t tick = 0; tick <= MAX_TICKS; tick++)
    {
        while (next_event < scenario.count &&
               scenario.events[next_event].tick <= tick)
        {
            scenario_event_t e = scenario.events[next_event];

            temperature = e.temp;
            humidity = e.humidity;
            switches = e.switches;

            next_event++;
        }

        _iom361_set_sensor1(temperature, humidity);
        _iom361_set_switches(switches);

        time_t timestamp = scenario.start + (time_t)tick;

        reading_t reading;

        if (sensor_read(base, timestamp, &reading) != 0)
        {
            failed = 1;
            break;
        }

        uint32_t sw = iom361_readReg(base, SWITCHES_REG, &rc);

        if (rc != 0)
        {
            failed = 1;
            break;
        }

        bool door_closed = (sw & DOOR_SWITCH) != 0;
        bool door_was_closed =
            (previous_switches & DOOR_SWITCH) != 0;

        bool start_pressed =
            (sw & START_SWITCH) != 0 &&
            (previous_switches & START_SWITCH) == 0;

        washer_event_t event = EVENT_COUNT;

        if (state != STATE_IDLE && !door_closed)
        {
            event = EVENT_DOOR_OPEN;
        }
        else if (state == STATE_IDLE &&
                 start_pressed && door_closed)
        {
            event = EVENT_START;
            started = true;
            finished = false;
        }
        else if (door_closed && !door_was_closed)
        {
            event = EVENT_DOOR_CLOSED;
        }
        else if (state != STATE_IDLE &&
                 tick - phase_start >= phase_duration(state))
        {
            event = completion_event(state);
        }

        if (event != EVENT_COUNT)
        {
            washer_state_t next =
                fsm_next_state(state, event);

            if (next != state)
            {
                state = next;
                phase_start = tick;

                aux_event_t history_event = {
                    tick, state
                };

                if (aux_push(history, history_event) != 0)
                {
                    failed = 1;
                    break;
                }

                if (state == STATE_IDLE)
                    finished = true;
            }
        }

        uint32_t leds =
            led_value(state, reading.temp_c, &heater);

        uint32_t rgb = rgb_value(state);

        iom361_writeReg(base, LEDS_REG, leds, &rc);

        if (rc != 0)
        {
            failed = 1;
            break;
        }

        iom361_writeReg(base, RGB_LED_REG, rgb, &rc);

        if (rc != 0)
        {
            failed = 1;
            break;
        }

        uint32_t status = pack_status(state, leds);

        /* Check that the status word unpacks correctly. */
        uint32_t decoded_state;
        uint32_t decoded_leds;
        uint32_t decoded_locked;

        unpack_status(status, &decoded_state,
                      &decoded_leds, &decoded_locked);

        if (decoded_state != (uint32_t)state ||
            decoded_leds != leds ||
            decoded_locked != (state != STATE_IDLE ? 1u : 0u) ||
            get_field(status, 8, 24) != 0)
        {
            failed = 1;
            break;
        }

        record_t record = {
            .reading = reading,
            .state = state,
            .status = status
        };

        if (store_insert(store, &record) != 0)
        {
            failed = 1;
            break;
        }

        if (!report_only)
        {
            print_trace(timestamp, state,
                        reading, leds, status);
        }

        previous_switches = sw;

        if (tick >= last_input &&
            (finished || (!started && state == STATE_IDLE)))
        {
            break;
        }

        if (tick == MAX_TICKS)
        {
            fprintf(stderr, "Simulation tick limit reached.\n");
            failed = 1;
        }
    }

    if (!failed && report_generate(report, store) != 0)
        failed = 1;

    if (!failed)
    {
        if (report_only)
        {
            report_print(report, stdout, scenario.start);
        }
        else
        {
            print_history(history);
        }
    }

    report_destroy(report);
    aux_destroy(history);
    store_destroy(store);
    scenario_free(&scenario);

    if (failed)
    {
        fprintf(stderr, "Simulation failed.\n");
        return 1;
    }

    return 0;
}
