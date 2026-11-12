#ifndef ISTACK_H
#define ISTACK_H
#include <stdbool.h>
#include <stddef.h>
#define ISTACK_START 8
typedef struct istack istack_t;
/* Creates an empty stack with initial room; returns NULL on allocation failure. */
istack_t *istack_create(void);
/* Frees the stack and its data; accepts NULL. */
void istack_destroy(istack_t *s);
/* Adds v to the top; returns 0 on success, -1 on NULL or allocation failure. */
int istack_push(istack_t *s, int v);
/* Removes top into v if non-NULL; returns -1 for NULL/empty and leaves v unchanged. */
int istack_pop(istack_t *s, int *v);
/* Reads top into v if non-NULL; returns -1 for NULL/empty and leaves v unchanged. */
int istack_peek(const istack_t *s, int *v);
/* Returns number of stored values, or 0 for NULL. */
size_t istack_count(const istack_t *s);
/* Returns true for NULL or an empty stack. */
bool istack_is_empty(const istack_t *s);
/* Returns allocated number of slots, or 0 for NULL. */
size_t istack_capacity(const istack_t *s);
/* Copies values bottom first; sets *n to count, or 0 for NULL/empty/failure. Caller frees result. */
int *istack_to_array(const istack_t *s, size_t *n);
#endif
