
#include <stdio.h>
#include "../report.h"

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK(condition) do {                         \
    tests_run++;                                      \
    if (condition) {                                  \
        printf("PASS: %s\n", #condition);             \
    } else {                                          \
        printf("FAIL: %s (line %d)\n",                \
               #condition, __LINE__);                 \
        tests_failed++;                               \
    }                                                 \
} while (0)

/* Create a washing machine record. */
static record_t make_record(
    washer_state_t state,
    time_t timestamp,
    float temperature)
{
    record_t record = {0};

    record.state = state;
    record.reading.timestamp = timestamp;
    record.reading.temp_c = temperature;
    record.reading.rh_pct = 50.0f;

    return record;
}

/* Check a report entry. */
static void check_entry(
    const report_t *report,
    size_t index,
    time_t timestamp,
    float temperature)
{
    report_entry_t entry = {0};

    int result = report_get(report, index, &entry);

    CHECK(result == 0);

    if (result == 0)
    {
        CHECK(entry.timestamp == timestamp);
        CHECK(entry.temperature == temperature);
    }
}

int main(void)
{
    printf("Testing Washing Machine Reports\n\n");

    store_t *store = store_create();
    report_t *report = report_create(3);

    CHECK(store != NULL);
    CHECK(report != NULL);

    if (store == NULL || report == NULL)
    {
        store_destroy(store);
        report_destroy(report);
        return 1;
    }

    /* Test 1: Empty store */
    CHECK(report_generate(report, store) == 0);
    CHECK(report_count(report) == 0);

    report_entry_t entry = {0};
    CHECK(report_get(report, 0, &entry) != 0);

    /* Test 2: Fewer than three WASH readings */
    record_t a = make_record(STATE_WASH, 100, 35.0f);
    record_t b = make_record(STATE_WASH, 101, 38.0f);

    CHECK(store_insert(store, &a) == 0);
    CHECK(store_insert(store, &b) == 0);

    CHECK(report_generate(report, store) == 0);
    CHECK(report_count(report) == 2);

    check_entry(report, 0, 101, 38.0f);
    check_entry(report, 1, 100, 35.0f);

    /* Test 3: Exactly three WASH readings */
    record_t c = make_record(STATE_WASH, 102, 40.0f);

    CHECK(store_insert(store, &c) == 0);
    CHECK(report_generate(report, store) == 0);
    CHECK(report_count(report) == 3);

    check_entry(report, 0, 102, 40.0f);
    check_entry(report, 1, 101, 38.0f);
    check_entry(report, 2, 100, 35.0f);

    /* Test 4: More than three readings */
    record_t d = make_record(STATE_WASH, 103, 45.0f);
    record_t e = make_record(STATE_WASH, 104, 42.0f);

    CHECK(store_insert(store, &d) == 0);
    CHECK(store_insert(store, &e) == 0);

    CHECK(report_generate(report, store) == 0);
    CHECK(report_count(report) == 3);

    check_entry(report, 0, 103, 45.0f);
    check_entry(report, 1, 104, 42.0f);
    check_entry(report, 2, 102, 40.0f);

    /* Test 5: Equal temperatures use earlier timestamps */
    record_t f = make_record(STATE_WASH, 105, 45.0f);
    record_t g = make_record(STATE_WASH, 99, 45.0f);

    CHECK(store_insert(store, &f) == 0);
    CHECK(store_insert(store, &g) == 0);

    CHECK(report_generate(report, store) == 0);
    CHECK(report_count(report) == 3);

    check_entry(report, 0, 99, 45.0f);
    check_entry(report, 1, 103, 45.0f);
    check_entry(report, 2, 105, 45.0f);

    /* Test 6: Ignore readings outside WASH */
    record_t h = make_record(STATE_RINSE, 106, 100.0f);
    record_t i = make_record(STATE_SPIN, 107, 99.0f);

    CHECK(store_insert(store, &h) == 0);
    CHECK(store_insert(store, &i) == 0);

    CHECK(report_generate(report, store) == 0);
    CHECK(report_count(report) == 3);

    check_entry(report, 0, 99, 45.0f);
    check_entry(report, 1, 103, 45.0f);
    check_entry(report, 2, 105, 45.0f);

    /* Test 7: Invalid report index */
    CHECK(report_get(report, 3, &entry) != 0);

    /* Free the data structures */
    report_destroy(report);
    store_destroy(store);

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
