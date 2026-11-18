#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct bst bst_t; /* Opaque tree type. */

/* Takes no arguments; returns a new empty tree or NULL on allocation failure. */
bst_t *bst_create(void);
/* Takes a tree pointer; frees all nodes and the tree; NULL is allowed. */
void bst_destroy(bst_t *t);
/* Takes a tree and key; inserts duplicates to the right; returns 0 on success,
 * -1 for NULL tree or allocation failure (tree unchanged on failure). */
int bst_insert(bst_t *t, int key);
/* Takes a tree, key, and optional visits output; returns whether key exists.
 * Visits counts compared nodes; NULL tree gives false and zero visits. */
bool bst_contains(const bst_t *t, int key, size_t *visits);
/* Takes a tree; returns its key count, or zero for NULL. */
size_t bst_size(const bst_t *t);
/* Takes a tree; returns longest root-to-leaf path in nodes, or zero for NULL. */
size_t bst_height(const bst_t *t);
/* Bonus: takes a tree and key; removes one matching key, returns 0 on success
 * or -1 if the tree is NULL or the key is absent (unchanged in that case). */
int bst_remove(bst_t *t, int key);

#endif
