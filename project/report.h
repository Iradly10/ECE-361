
#ifndef REPORT_H
#define REPORT_H

#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "store.h"

/* One entry in the temperature report. */
typedef struct {
    time_t timestamp;
    float temperature;
} report_entry_t;

/* Opaque report structure. */
typedef struct report report_t;

/* Create a report for the k highest WASH temperatures. */
report_t *report_create(size_t k);

/* Collect the highest temperatures from the store. */
int report_generate(report_t *report, const store_t *store);

/* Get report information. */
size_t report_count(const report_t *report);

int report_get(const report_t *report,
               size_t index,
               report_entry_t *entry);

/* Print the report. */
void report_print(const report_t *report,
                  FILE *output,
                  time_t start_time);

/* Free the report. */
void report_destroy(report_t *report);

#endif
