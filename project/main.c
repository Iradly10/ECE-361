#include <stdio.h>
#include "iom361_r4.h"

int main(void)
{
    int rc = 0;

    uint32_t *base = iom361_initialize(2, 4, &rc);

    if (base == NULL || rc != 0)
    {
        fprintf(stderr, "Initialization failed: %d\n", rc);
        return 1;
    }

    printf("ECE 361 Washing Machine Project\n");

    return 0;
}