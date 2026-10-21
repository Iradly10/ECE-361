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