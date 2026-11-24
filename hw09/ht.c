#include "ht.h"
#include <stdlib.h>
#include <stdint.h>

struct node {
    int key;
    size_t count;
    struct node *next;
};

struct ht {
    size_t nbuckets;
    size_t size;
    struct node **buckets;
};

ht_t *ht_create(size_t nbuckets)
{
    if (nbuckets == 0 || nbuckets > SIZE_MAX / sizeof(struct node *))
        return NULL;
    ht_t *t = malloc(sizeof *t);
    if (!t)
        return NULL;
    t->buckets = calloc(nbuckets, sizeof *t->buckets);
    if (!t->buckets) {
        free(t);
        return NULL;
    }
    t->nbuckets = nbuckets;
    t->size = 0;
    return t;
}

void ht_destroy(ht_t *t)
{
    if (!t)
        return;
    for (size_t i = 0; i < t->nbuckets; ++i) {
        struct node *p = t->buckets[i];
        while (p) {
            struct node *next = p->next;
            free(p);
            p = next;
        }
    }
    free(t->buckets);
    free(t);
}

int ht_add(ht_t *t, int key)
{
    if (!t || key < 0)
        return -1;
    size_t index = (size_t)key % t->nbuckets;
    for (struct node *p = t->buckets[index]; p; p = p->next) {
        if (p->key == key) {
            ++p->count;
            return 0;
        }
    }
    /* Allocate before changing the table so failure leaves it unchanged. */
    struct node *p = malloc(sizeof *p);
    if (!p)
        return -1;
    p->key = key;
    p->count = 1;
    p->next = t->buckets[index];
    t->buckets[index] = p;
    ++t->size;
    return 0;
}

size_t ht_count(const ht_t *t, int key)
{
    if (!t || key < 0)
        return 0;
    size_t index = (size_t)key % t->nbuckets;
    for (const struct node *p = t->buckets[index]; p; p = p->next)
        if (p->key == key)
            return p->count;
    return 0;
}

size_t ht_size(const ht_t *t)
{
    return t ? t->size : 0;
}

size_t ht_longest_chain(const ht_t *t)
{
    if (!t)
        return 0;
    size_t longest = 0;
    for (size_t i = 0; i < t->nbuckets; ++i) {
        size_t length = 0;
        for (const struct node *p = t->buckets[i]; p; p = p->next)
            ++length;
        if (length > longest)
            longest = length;
    }
    return longest;
}
