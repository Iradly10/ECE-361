
#include "scenario.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <inttypes.h>

// Convert a timestamp into UTC time without depending
// on the computer's local timezone.
static int parse_timestamp(const char *text, time_t *result)
{
    int year, month, day, hour, minute, second;
    int used = 0;

    if (sscanf(text, "%d-%d-%d %d:%d:%d%n",
               &year, &month, &day,
               &hour, &minute, &second, &used) != 6) {
        return 0;
    }

    if (text[used] != '\0' ||
        month < 1 || month > 12 ||
        day < 1 || day > 31 ||
        hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 ||
        second < 0 || second > 59) {
        return 0;
    }

    // Count days since January 1, 1970.
    int y = year - (month <= 2);
    int era = (y >= 0 ? y : y - 399) / 400;
    unsigned int yoe = (unsigned int)(y - era * 400);
    int mp = month + (month > 2 ? -3 : 9);
    unsigned int doy =
        (153u * (unsigned int)mp + 2u) / 5u +
        (unsigned int)day - 1u;
    unsigned int doe =
        yoe * 365u + yoe / 4u - yoe / 100u + doy;

    int64_t days =
        (int64_t)era * 146097 + (int64_t)doe - 719468;

    int64_t seconds =
        days * 86400 + hour * 3600 + minute * 60 + second;

    *result = (time_t)seconds;

    // Verify that the date is a real calendar date.
    struct tm *check = gmtime(result);

    if (check == NULL ||
        check->tm_year + 1900 != year ||
        check->tm_mon + 1 != month ||
        check->tm_mday != day ||
        check->tm_hour != hour ||
        check->tm_min != minute ||
        check->tm_sec != second) {
        return 0;
    }

    return 1;
}

// Read and validate a scenario file.
int scenario_load(const char *filename, scenario_t *scenario)
{
    if (filename == NULL || scenario == NULL) {
        return 1;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 1;
    }

    scenario_t temp = {0};
    char buffer[512];
    int line_number = 0;
    int have_start = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_number++;

        size_t length = strlen(buffer);

        if (length == sizeof(buffer) - 1 &&
            buffer[length - 1] != '\n' &&
            !feof(file)) {
            fclose(file);
            return line_number;
        }

        // Remove comments beginning with #.
        char *comment = strchr(buffer, '#');

        if (comment != NULL) {
            *comment = '\0';
        }

        // Skip leading whitespace.
        char *p = buffer;

        while (isspace((unsigned char)*p)) {
            p++;
        }

        if (*p == '\0') {
            continue;
        }

        if (!have_start) {
            char date[32];
            char clock[32];
            char extra;
            int fields = sscanf(p, "start %31s %31s %c",
                                date, clock, &extra);

            if (fields != 2) {
                fclose(file);
                return line_number;
            }

            char timestamp[80];

            snprintf(timestamp, sizeof(timestamp),
                     "%s %s", date, clock);

            if (!parse_timestamp(timestamp, &temp.start)) {
                fclose(file);
                return line_number;
            }

            have_start = 1;
            continue;
        }

        if (temp.count >= SCENARIO_MAX_LINES) {
            fclose(file);
            return line_number;
        }

        scenario_line_t entry;
        char switch_text[64];
        char extra;

        int fields = sscanf(p, "%u %f %f %63s %c",
                            &entry.tick,
                            &entry.temp_c,
                            &entry.rh_pct,
                            switch_text,
                            &extra);

        if (fields != 4) {
            fclose(file);
            return line_number;
        }

        // Switch values must be hexadecimal.
        if (strlen(switch_text) < 3 ||
            switch_text[0] != '0' ||
            (switch_text[1] != 'x' &&
             switch_text[1] != 'X')) {
            fclose(file);
            return line_number;
        }

        errno = 0;
        char *end = NULL;
        unsigned long switches =
            strtoul(switch_text, &end, 16);

        if (errno != 0 || *end != '\0' ||
            switches > UINT32_MAX) {
            fclose(file);
            return line_number;
        }

        entry.switches = (uint32_t)switches;

        // Require the first tick to be zero.
        if (temp.count == 0 && entry.tick != 0) {
            fclose(file);
            return line_number;
        }

        // Later ticks must strictly increase.
        if (temp.count > 0 &&
            entry.tick <= temp.lines[temp.count - 1].tick) {
            fclose(file);
            return line_number;
        }

        temp.lines[temp.count] = entry;
        temp.count++;
    }

    fclose(file);

    if (!have_start || temp.count == 0) {
        return line_number > 0 ? line_number : 1;
    }

    *scenario = temp;
    return 0;
}

// Find the latest scenario line at or before tick.
const scenario_line_t *scenario_at(const scenario_t *scenario,
                                   unsigned int tick)
{
    if (scenario == NULL || scenario->count == 0) {
        return NULL;
    }

    if (tick < scenario->lines[0].tick) {
        return NULL;
    }

    size_t left = 0;
    size_t right = scenario->count;

    while (left + 1 < right) {
        size_t mid = left + (right - left) / 2;

        if (scenario->lines[mid].tick <= tick) {
            left = mid;
        } else {
            right = mid;
        }
    }

    return &scenario->lines[left];
}
