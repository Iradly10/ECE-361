
#include "report.h"
#include <stdlib.h>
#include <stdint.h>

/* Internal report structure. */
struct report {
    report_entry_t *entries;
    size_t count;
    size_t k;
};

/* Create a report with room for k readings. */
report_t *report_create(size_t k)
{
    if (k > SIZE_MAX / sizeof(report_entry_t))
        return NULL;

    report_t *report = malloc(sizeof(*report));

    if (report == NULL)
        return NULL;

    report->entries = NULL;
    report->count = 0;
    report->k = k;

    if (k > 0)
    {
        report->entries = malloc(
            k * sizeof(*report->entries)
        );

        if (report->entries == NULL)
        {
            free(report);
            return NULL;
        }
    }

    return report;
}

/* Compare entries: highest temperature first,
   then earliest timestamp first. */
static int entry_before(report_entry_t a,
                        report_entry_t b)
{
    if (a.temperature > b.temperature)
        return 1;

    if (a.temperature < b.temperature)
        return 0;

    return a.timestamp < b.timestamp;
}

/* Add a temperature to the top-k list. */
static void report_add(report_t *report,
                       report_entry_t entry)
{
    if (report->k == 0)
        return;

    size_t pos = 0;

    while (pos < report->count &&
           !entry_before(entry, report->entries[pos]))
    {
        pos++;
    }

    if (pos >= report->k)
        return;

    if (report->count < report->k)
        report->count++;

    for (size_t i = report->count - 1; i > pos; i--)
    {
        report->entries[i] = report->entries[i - 1];
    }

    report->entries[pos] = entry;
}

/* Callback used to examine stored records. */
static void collect_record(const record_t *record,
                           void *context)
{
    report_t *report = context;

    if (record->state != STATE_WASH)
        return;

    report_entry_t entry;

    entry.timestamp = record->reading.timestamp;
    entry.temperature = record->reading.temp_c;

    report_add(report, entry);
}

/* Generate the report from the binary search tree. */
int report_generate(report_t *report, const store_t *store)
{
    if (report == NULL || store == NULL)
        return -1;

    report->count = 0;

    store_inorder(store, collect_record, report);

    return 0;
}

/* Return the number of report entries. */
size_t report_count(const report_t *report)
{
    if (report == NULL)
        return 0;

    return report->count;
}

/* Retrieve an entry by index. */
int report_get(const report_t *report,
               size_t index,
               report_entry_t *entry)
{
    if (report == NULL || entry == NULL ||
        index >= report->count)
    {
        return -1;
    }

    *entry = report->entries[index];
    return 0;
}

/* Print the highest WASH temperatures. */
void report_print(const report_t *report,
                  FILE *output,
                  time_t start_time)
{
    if (report == NULL || output == NULL)
        return;

    fprintf(output, "Highest WASH temperatures:\n");

    if (report->count == 0)
    {
        fprintf(output, "No WASH readings recorded.\n");
        return;
    }

    for (size_t i = 0; i < report->count; i++)
    {
        const report_entry_t *entry = &report->entries[i];

        double tick = difftime(entry->timestamp,
                               start_time);

        fprintf(output,
                "%zu. tick=%.0f temp=%.1f C\n",
                i + 1,
                tick,
                entry->temperature);
    }
}

/* Destroy the report and free memory. */
void report_destroy(report_t *report)
{
    if (report == NULL)
        return;

    free(report->entries);
    free(report);
}
