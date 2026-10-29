
#ifndef STORE_H
#define STORE_H

#include <stddef.h>
#include <stdint.h>
#include "sensor.h"
#include "fsm.h"

/* One washing machine record. */
typedef struct {
    reading_t reading;
    washer_state_t state;
    uint32_t status;
} record_t;

/* Opaque binary search tree. */
typedef struct store store_t;

/* Callback for visiting records. */
typedef void (*store_visit_fn)(
    const record_t *record,
    void *context
);

/* Compare records by state, then timestamp. */
int record_compare(const record_t *a, const record_t *b);

/* Create and destroy the tree. */
store_t *store_create(void);
void store_destroy(store_t *store);

/* Insert and find records. */
int store_insert(store_t *store, const record_t *record);

const record_t *store_find(
    const store_t *store,
    const record_t *key
);

/* Visit records in sorted order. */
void store_inorder(
    const store_t *store,
    store_visit_fn visit,
    void *context
);

/* Tree statistics. */
size_t store_count(const store_t *store);
size_t store_height(const store_t *store);

#endif
