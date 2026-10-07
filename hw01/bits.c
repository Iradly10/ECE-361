/*
 * bits.c contains the implementation of bit manipulation functions.
 */

 #include "bits.h" // Gives us our function declarations and uint32_t/int32_t
 #include <stdio.h>  // Needed for printf()
 #include <stdlib.h>
// Prints a number in binary using the requested number of bits

 void print_binary(uint32_t x, int width)       // Print the binary representation of x with the specified width
{
     // Start at the leftmost bit and move toward bit 0
    for (int i = width - 1; i >= 0; i--)
    {
        // Move the bit we want to the right, then keep only that bit
        uint32_t bit = (x >> i) & 1u;
        // Print the 0 or 1
        printf("%u", bit);
    // Add a space every 4 bits
        if (i > 0 && i % 4 == 0)
        {
            printf(" ");
        }
    }
    // Move to the next line after printing all bits
    printf("\n");
}
// Gets a group of bits from a 32-bit word
uint32_t get_field(uint32_t word, int pos, int width)    // Extract a field of the specified width from the word starting at the specified position          
{
    //Check if position or width is invalid
    if (width < 1 || width > 32 ||      // Check for valid width and position
        pos < 0 || pos > 31 || 
        pos + width > 32)
    {
        return 0;  // My choice for invalid input
    }
    // Special case: asking for all 32 bits
    // Avoids doing 1u << 32, which is not valid
    if (width == 32)
    {
        return word;
    }
    // Create a mask with 'width' number of 1s
    // Example: width = 3 -> mask = 00000111
    uint32_t mask = (1u << width) - 1u;
    // Move wanted bits to the right, then keep only those bits
    return (word >> pos) & mask;
}
// Replaces a group of bits inside a 32-bit word
uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    // Check if position or width is invalid
    if (width < 1 || width > 32 ||      // Check for valid width and position   
        pos < 0 || pos > 31 ||
        pos + width > 32)
    {
        return word; // Invalid input -> leave original word unchanged
    }
    // Special case: replacing all 32 bits
    // Avoids doing 1u << 32, which is not valid
    if (width == 32)
    {
        return value;
    }
    // Create a mask with 'width' number of 1s
    // Example: width = 4 -> 00001111
    uint32_t mask = (1u << width) - 1u;
    // Clear the old bits in the field (turn them into 0s)
    word &= ~(mask << pos);
    // Put the new value into that field
    // value & mask makes sure we only use the needed bits
    word |= (value & mask) << pos;
    // Return the modified word
    return word;
}
// Converts a smaller signed bit pattern into a signed 32-bit value
int32_t sign_extend(uint32_t value, int width)
{
    // Make sure width is valid
    if (width < 1 || width > 32)
    {
        return 0;
    }
    // Already 32 bits, so just treat it as a signed 32-bit number
    if (width == 32)
    {
        return (int32_t)value;
    }
    // Create a mask for the requested width
    uint32_t mask = (1u << width) - 1u;
    // Remove any bits outside the requested width
    value &= mask;
    // Find the sign bit (leftmost bit of the requested width)
    uint32_t sign_bit = 1u << (width - 1);

    if (value & sign_bit)
    {
        // Fill the upper bits with 1s to keep it negative
        value |= ~mask;
    }
    // Return the result as a signed 32-bit integer
    return (int32_t)value;
}