#include <stdio.h>
#include <string.h>
#include "tally.h"


int main(void)
{
    tally_t *t = tally_create();

    if (t == NULL)
    {
        fprintf(stderr, "Could not create tally\n");
        return 1;
    }
    char line[256];
    int skipped = 0;

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0' || line[0] == '#')
        {
            continue;
        }
        if (tally_add(t, line) == -1)
        {
            skipped++;
        }
    }
    tally_print(t, stdout);
    printf("names: %d, lines skipped: %d\n", tally_size(t), skipped);
    tally_destroy(t);
    return 0;
}