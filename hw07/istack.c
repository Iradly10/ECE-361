#include "istack.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
struct istack { int *data; size_t count; size_t capacity; };
istack_t *istack_create(void) {
    istack_t *s = malloc(sizeof *s);
    if (!s) return NULL;
    s->data = malloc(ISTACK_START * sizeof *s->data);
    if (!s->data) { free(s); return NULL; }
    s->count = 0;
    s->capacity = ISTACK_START;
    return s;
}
void istack_destroy(istack_t *s) {
    if (!s) return;
    free(s->data);
    free(s);
}
int istack_push(istack_t *s, int v) {
    if (!s) return -1;
    if (s->count == s->capacity) {
        if (s->capacity > SIZE_MAX / 2 || s->capacity * 2 > SIZE_MAX / sizeof *s->data) return -1;
        size_t next_capacity = s->capacity * 2;
        int *new_data = realloc(s->data, next_capacity * sizeof *s->data);
        if (!new_data) return -1;
        s->data = new_data;
        s->capacity = next_capacity;
    }
    s->data[s->count++] = v;
    return 0;
}
int istack_pop(istack_t *s, int *v) {
    if (!s || s->count == 0) return -1;
    int value = s->data[--s->count];
    if (v) *v = value;
    return 0;
}
int istack_peek(const istack_t *s, int *v) {
    if (!s || s->count == 0) return -1;
    if (v) *v = s->data[s->count - 1];
    return 0;
}
size_t istack_count(const istack_t *s) { return s ? s->count : 0; }
bool istack_is_empty(const istack_t *s) { return !s || s->count == 0; }
size_t istack_capacity(const istack_t *s) { return s ? s->capacity : 0; }
int *istack_to_array(const istack_t *s, size_t *n) {
    if (n) *n = 0;
    if (!s || !s->count) return NULL;
    if (s->count > SIZE_MAX / sizeof *s->data) return NULL;
    int *copy = malloc(s->count * sizeof *copy);
    if (!copy) return NULL;
    memcpy(copy, s->data, s->count * sizeof *copy);
    if (n) *n = s->count;
    return copy;
}
