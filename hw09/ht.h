#ifndef HT_H
#define HT_H
#include <stddef.h>

typedef struct ht ht_t;
/* Create an empty table with nbuckets buckets; return NULL on zero or allocation failure. */
ht_t *ht_create(size_t nbuckets);
/* Free the table and all its nodes; NULL is allowed. */
void ht_destroy(ht_t *t);
/* Add one occurrence of a nonnegative key; return 0 or -1 on NULL/allocation failure. */
int ht_add(ht_t *t, int key);
/* Return the count of a nonnegative key, or zero if absent or t is NULL. */
size_t ht_count(const ht_t *t, int key);
/* Return the number of distinct keys, or zero for NULL. */
size_t ht_size(const ht_t *t);
/* Return the maximum number of nodes in any bucket, or zero for empty/NULL. */
size_t ht_longest_chain(const ht_t *t);
#endif
