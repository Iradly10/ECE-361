/*
 * bits.c contains the implementation of bit manipulation functions.
 */

 #include "bits.h"
 #include <stdio.h>
 #include <stdlib.h>

 void print_binary(uint32_t x, int width)
{
    for (int i = width - 1; i >= 0; i--)
    {
        uint32_t bit = (x >> i) & 1u;
        printf("%u", bit);

        if (i > 0 && i % 4 == 0)
        {
            printf(" ");
        }
    }

    printf("\n");
}

uint32_t get_field(uint32_t word, int pos, int width)
{
    if (width < 1 || width > 32 ||
        pos < 0 || pos > 31 ||
        pos + width > 32)
    {
        return 0;
    }

    if (width == 32)
    {
        return word;
    }

    uint32_t mask = (1u << width) - 1u;

    return (word >> pos) & mask;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    if (width < 1 || width > 32 ||
        pos < 0 || pos > 31 ||
        pos + width > 32)
    {
        return word;
    }

    if (width == 32)
    {
        return value;
    }

    uint32_t mask = (1u << width) - 1u;

    word &= ~(mask << pos);

    word |= (value & mask) << pos;

    return word;
}

int32_t sign_extend(uint32_t value, int width)
{
    if (width < 1 || width > 32)
    {
        return 0;
    }

    if (width == 32)
    {
        return (int32_t)value;
    }

    uint32_t mask = (1u << width) - 1u;
    value &= mask;

    uint32_t sign_bit = 1u << (width - 1);

    if (value & sign_bit)
    {
        value |= ~mask;
    }

    return (int32_t)value;
}