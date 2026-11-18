#include "bst.h"
#include <stdlib.h>

/* The original's struct node stays private to this implementation. */
struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

struct bst {
    struct node *root;
    size_t count;
};

bst_t *bst_create(void) {
    return calloc(1, sizeof(bst_t));
}

/* Added cleanup; the original had no destroy function. */
static void destroy_nodes(struct node *current) {
    if (current == NULL) return;
    destroy_nodes(current->leftChild);
    destroy_nodes(current->rightChild);
    free(current);
}

void bst_destroy(bst_t *t) {
    if (t == NULL) return;
    destroy_nodes(t->root);
    free(t);
}

/* Adapted from the original insert(): use t->root instead of global root. */
int bst_insert(bst_t *t, int key) {
    if (t == NULL) return -1;
    struct node *tempNode = malloc(sizeof(*tempNode));
    if (tempNode == NULL) return -1;
    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    if (t->root == NULL) {
        t->root = tempNode;
    } else {
        struct node *current = t->root;
        struct node *parent = NULL;
        while (1) {
            parent = current;
            if (key < parent->data) {
                current = current->leftChild;
                if (current == NULL) {
                    parent->leftChild = tempNode;
                    break;
                }
            } else {
                current = current->rightChild;
                if (current == NULL) {
                    parent->rightChild = tempNode;
                    break;
                }
            }
        }
    }
    ++t->count;
    return 0;
}

/* Adapted from original search(): check current before dereferencing it.
 * The original while(current->data != data) crashed for an empty tree. */
bool bst_contains(const bst_t *t, int key, size_t *visits) {
    if (visits != NULL) *visits = 0;
    const struct node *current = t == NULL ? NULL : t->root;
    while (current != NULL) {
        if (visits != NULL) ++*visits;
        if (current->data == key) return true;
        if (current->data > key)
            current = current->leftChild;
        else
            current = current->rightChild;
    }
    return false;
}

size_t bst_size(const bst_t *t) {
    return t == NULL ? 0 : t->count;
}

static size_t node_height(const struct node *current) {
    if (current == NULL) return 0;
    size_t left = node_height(current->leftChild);
    size_t right = node_height(current->rightChild);
    return 1 + (left > right ? left : right);
}

size_t bst_height(const bst_t *t) {
    return t == NULL ? 0 : node_height(t->root);
}

/* Bonus: follows the original search's smaller-left/otherwise-right rule. */
int bst_remove(bst_t *t, int key) {
    if (t == NULL) return -1;
    struct node **link = &t->root;
    while (*link != NULL && (*link)->data != key) {
        if (key < (*link)->data)
            link = &(*link)->leftChild;
        else
            link = &(*link)->rightChild;
    }
    if (*link == NULL) return -1;
    struct node *target = *link;
    if (target->leftChild != NULL && target->rightChild != NULL) {
        struct node **successor_link = &target->rightChild;
        while ((*successor_link)->leftChild != NULL)
            successor_link = &(*successor_link)->leftChild;
        struct node *successor = *successor_link;
        target->data = successor->data;
        *successor_link = successor->rightChild;
        free(successor);
    } else {
        *link = target->leftChild != NULL ? target->leftChild : target->rightChild;
        free(target);
    }
    --t->count;
    return 0;
}
