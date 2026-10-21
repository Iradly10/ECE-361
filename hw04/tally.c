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