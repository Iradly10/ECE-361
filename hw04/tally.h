#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX 4
#define TALLY_NAMES 16
#define TALLY_LEN 15

typedef struct tally tally_t;

/* Creates an empty tally.
   Returns NULL if all tallies are in use.
   Takes no arguments. */
tally_t *tally_create(void);

/* Destroys a tally so it can be reused.
   Takes a tally pointer. NULL does nothing. */
void tally_destroy(tally_t *t);

/* Adds one occurrence of a name.
   Returns the new count, or -1 for invalid input
   or when a new name cannot fit. */
int tally_add(tally_t *t, const char *name);

/* Gets the count of a name.
   Returns 0 if the name is missing or an argument is NULL. */
int tally_count(const tally_t *t, const char *name);

/* Gets the number of different names.
   Returns 0 if the tally is NULL. */
int tally_size(const tally_t *t);

/* Prints names and counts in alphabetical order.
   Takes a tally and an output file.
   Assumes both pointers are valid. */
void tally_print(const tally_t *t, FILE *out);

#endif