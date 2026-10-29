
#include <stdio.h>
#include <stddef.h>
#include "../store.h"

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK(condition) do {                          \
    tests_run++;                                       \
    if (condition) {                                   \
        printf("PASS: %s\n", #condition);              \
    } else {                                           \
        printf("FAIL: %s (line %d)\n",                 \
               #condition, __LINE__);                  \
        tests_failed++;                                \
    }                                                  \
} while (0)

/* Create a record with a state and timestamp. */
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
    record.status = 0;

    return record;
}

/* Keep track of records visited during inorder traversal. */
typedef struct {
    size_t count;
    int sorted;
    int has_previous;
    record_t previous;
} visit_context_t;

static void check_order(
    const record_t *record,
    void *context)
{
    visit_context_t *ctx = context;

    if (ctx->has_previous &&
        record_compare(&ctx->previous, record) > 0)
    {
        ctx->sorted = 0;
    }

    ctx->previous = *record;
    ctx->has_previous = 1;
    ctx->count++;
}

int main(void)
{
    printf("Testing Washing Machine BST\n\n");

    /* Test record comparison. */
    record_t a = make_record(STATE_WASH, 100, 30.0f);
    record_t b = make_record(STATE_WASH, 200, 35.0f);
    record_t c = make_record(STATE_RINSE, 100, 25.0f);
    record_t d = make_record(STATE_WASH, 100, 40.0f);

    CHECK(record_compare(&a, &b) < 0);
    CHECK(record_compare(&b, &a) > 0);
    CHECK(record_compare(&a, &d) == 0);
    CHECK(record_compare(&a, &c) < 0);
    CHECK(record_compare(&c, &a) > 0);

    /* Test an empty tree. */
    store_t *store = store_create();

    if (store == NULL)
    {
        printf("FAIL: store_create returned NULL\n");
        return 1;
    }

    CHECK(store_count(store) == 0);
    CHECK(store_height(store) == 0);
    CHECK(store_find(store, &a) == NULL);

    visit_context_t ctx = {0};
    ctx.sorted = 1;

    store_inorder(store, check_order, &ctx);
    CHECK(ctx.count == 0);

    /* Test insertion of one record. */
    CHECK(store_insert(store, &a) == 0);
    CHECK(store_count(store) == 1);
    CHECK(store_height(store) == 1);

    const record_t *found = store_find(store, &a);
    CHECK(found != NULL);

    if (found != NULL)
    {
        CHECK(found->reading.temp_c == 30.0f);
    }

    /* Test insertion of additional records. */
    CHECK(store_insert(store, &b) == 0);
    CHECK(store_insert(store, &c) == 0);

    CHECK(store_count(store) == 3);
    CHECK(store_find(store, &b) != NULL);
    CHECK(store_find(store, &c) != NULL);

    /* Check that a missing record is not found. */
    record_t missing = make_record(STATE_SPIN, 999, 20.0f);
    CHECK(store_find(store, &missing) == NULL);

    /* Test inorder traversal. */
    ctx = (visit_context_t){0};
    ctx.sorted = 1;

    store_inorder(store, check_order, &ctx);

    CHECK(ctx.count == 3);
    CHECK(ctx.sorted == 1);

    /* Destroy a nonempty tree. */
    store_destroy(store);

    /* Test a tree with 100 records. */
    store = store_create();

    if (store == NULL)
    {
        printf("FAIL: second store_create returned NULL\n");
        return 1;
    }

    for (int i = 0; i < 100; i++)
    {
        record_t record = make_record(
            STATE_WASH,
            (time_t)(1000 + i),
            (float)i
        );

        if (store_insert(store, &record) != 0)
        {
            printf("FAIL: inserting record %d\n", i);
            tests_failed++;
            break;
        }
    }

    CHECK(store_count(store) == 100);
    CHECK(store_height(store) >= 1);
    CHECK(store_height(store) <= 100);

    record_t first = make_record(STATE_WASH, 1000, 0.0f);
    record_t last = make_record(STATE_WASH, 1099, 99.0f);

    CHECK(store_find(store, &first) != NULL);
    CHECK(store_find(store, &last) != NULL);

    ctx = (visit_context_t){0};
    ctx.sorted = 1;

    store_inorder(store, check_order, &ctx);

    CHECK(ctx.count == 100);
    CHECK(ctx.sorted == 1);

    store_destroy(store);

    /* Destroy an empty tree. */
    store = store_create();
    CHECK(store != NULL);
    store_destroy(store);

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
