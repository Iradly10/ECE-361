
#include "store.h"
#include <stdlib.h>

/* A node in the binary search tree. */
typedef struct node {
    record_t record;
    struct node *left;
    struct node *right;
} node_t;

/* Internal structure of the store. */
struct store {
    node_t *root;
    size_t count;
};

/* Compare by state first, then timestamp. */
int record_compare(const record_t *a, const record_t *b)
{
    if (a == NULL || b == NULL)
        return 0;

    if (a->state < b->state)
        return -1;

    if (a->state > b->state)
        return 1;

    if (a->reading.timestamp < b->reading.timestamp)
        return -1;

    if (a->reading.timestamp > b->reading.timestamp)
        return 1;

    return 0;
}

/* Create an empty tree. */
store_t *store_create(void)
{
    store_t *store = malloc(sizeof(*store));

    if (store == NULL)
        return NULL;

    store->root = NULL;
    store->count = 0;

    return store;
}

/* Create a new node. */
static node_t *node_create(const record_t *record)
{
    node_t *node = malloc(sizeof(*node));

    if (node == NULL)
        return NULL;

    node->record = *record;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* Insert a record into the tree. */
int store_insert(store_t *store, const record_t *record)
{
    if (store == NULL || record == NULL)
        return -1;

    node_t **current = &store->root;

    while (*current != NULL)
    {
        int cmp = record_compare(record,
                                 &(*current)->record);

        if (cmp == 0)
        {
            (*current)->record = *record;
            return 0;
        }

        if (cmp < 0)
            current = &(*current)->left;
        else
            current = &(*current)->right;
    }

    *current = node_create(record);

    if (*current == NULL)
        return -1;

    store->count++;
    return 0;
}

/* Find a record by state and timestamp. */
const record_t *store_find(const store_t *store,
                           const record_t *key)
{
    if (store == NULL || key == NULL)
        return NULL;

    const node_t *current = store->root;

    while (current != NULL)
    {
        int cmp = record_compare(key, &current->record);

        if (cmp == 0)
            return &current->record;

        if (cmp < 0)
            current = current->left;
        else
            current = current->right;
    }

    return NULL;
}

/* Recursive in-order traversal. */
static void node_inorder(const node_t *node,
                        store_visit_fn visit,
                        void *context)
{
    if (node == NULL)
        return;

    node_inorder(node->left, visit, context);
    visit(&node->record, context);
    node_inorder(node->right, visit, context);
}

/* Visit all records in sorted order. */
void store_inorder(const store_t *store,
                   store_visit_fn visit,
                   void *context)
{
    if (store == NULL || visit == NULL)
        return;

    node_inorder(store->root, visit, context);
}

/* Return the number of records. */
size_t store_count(const store_t *store)
{
    if (store == NULL)
        return 0;

    return store->count;
}

/* Calculate the height of a node. */
static size_t node_height(const node_t *node)
{
    if (node == NULL)
        return 0;

    size_t left_height = node_height(node->left);
    size_t right_height = node_height(node->right);

    if (left_height > right_height)
        return left_height + 1;

    return right_height + 1;
}

/* Return the height of the tree. */
size_t store_height(const store_t *store)
{
    if (store == NULL)
        return 0;

    return node_height(store->root);
}

/* Recursively free all nodes. */
static void node_destroy(node_t *node)
{
    if (node == NULL)
        return;

    node_destroy(node->left);
    node_destroy(node->right);

    free(node);
}

/* Destroy the tree and release memory. */
void store_destroy(store_t *store)
{
    if (store == NULL)
        return;

    node_destroy(store->root);
    free(store);
}
