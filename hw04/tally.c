#include "tally.h"
#include <string.h>

typedef struct {
    char name[TALLY_LEN + 1];
    int count;
} entry_t;

struct tally {
    entry_t entries[TALLY_NAMES];
    int size;
    int in_use;
};

static tally_t pool[TALLY_MAX];

tally_t *tally_create(void)
{
    for (int i = 0; i < TALLY_MAX; i++)
    {
        if (pool[i].in_use == 0)
        {
            pool[i].in_use = 1;
            pool[i].size = 0;

            return &pool[i];
        }
    }

    return NULL;
}

void tally_destroy(tally_t *t)
{
    if (t == NULL)
    {
        return;
    }

    t->in_use = 0;
    t->size = 0;
}
int tally_add(tally_t *t, const char *name)
{
    if (t == NULL || name == NULL)
    {
        return -1;
    }

    size_t len = strlen(name);

    if (len == 0 || len > TALLY_LEN)
    {
        return -1;
    }
    for (int i = 0; i < t->size; i++)
    {
        if (strcmp(t->entries[i].name, name) == 0)
        {
            t->entries[i].count++;
            return t->entries[i].count;
        }
    }

    if (t->size >= TALLY_NAMES)
    {
        return -1;
    }
    int pos = 0;

    while (pos < t->size &&
           strcmp(t->entries[pos].name, name) < 0)
    {
        pos++;
    }
    for (int i = t->size; i > pos; i--)
    {
        t->entries[i] = t->entries[i - 1];
    }
    memcpy(t->entries[pos].name, name, len + 1);
    t->entries[pos].count = 1;
    t->size++;

    return 1;
}
int tally_count(const tally_t *t, const char *name)
{
    if (t == NULL || name == NULL)
    {
        return 0;
    }

    for (int i = 0; i < t->size; i++)
    {
        if (strcmp(t->entries[i].name, name) == 0)
        {
            return t->entries[i].count;
        }
    }

    return 0;
}
int tally_size(const tally_t *t)
{
    if (t == NULL)
    {
        return 0;
    }

    return t->size;
}
void tally_print(const tally_t *t, FILE *out)
{
    if (t == NULL || out == NULL)
    {
        return;
    }

    for (int i = 0; i < t->size; i++)
    {
        fprintf(out, "%s %d\n",
                t->entries[i].name,
                t->entries[i].count);
    }
}