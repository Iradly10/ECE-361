
#include "reading.h"

#define LINE_LEN 128

int reading_read(FILE *input,
                 int ticks[],
                 float temps[],
                 float hums[],
                 int *skipped)
{
    char line[LINE_LEN];
    int count = 0;

    *skipped = 0;

    while (fgets(line, sizeof line, input) != NULL)
    {
        int i = 0;

        /* Skip leading spaces and tabs */
        while (line[i] == ' ' || line[i] == '\t')
        {
            i++;
        }

        /* Ignore blank lines and comments */
        if (line[i] == '\n' ||
            line[i] == '\0' ||
            line[i] == '#')
        {
            continue;
        }

        /* Stop when maximum readings reached */
        if (count == MAX_READINGS)
        {
            fprintf(stderr,
                    "warning: more than %d readings, the rest are ignored\n",
                    MAX_READINGS);
            break;
        }

        /* Read tick, temperature, humidity */
        if (sscanf(line, "%d %f %f",
                   &ticks[count],
                   &temps[count],
                   &hums[count]) == 3)
        {
            count++;
        }
        else
        {
            (*skipped)++;
        }
    }

    return count;
}
