#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    bst_t *tree = bst_create();
    if (tree == NULL) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }
    int value;
    int status;
    size_t read_count = 0;
    while ((status = scanf("%d", &value)) == 1) {
        if (bst_insert(tree, value) != 0) {
            fprintf(stderr, "out of memory\n");
            bst_destroy(tree);
            return 1;
        }
        ++read_count;
    }
    if (status == 0) {
        printf("not a number after %zu values\n", read_count);
        bst_destroy(tree);
        return 1;
    }
    if (read_count == 0)
        puts("empty tree");
    else
        printf("n %zu, height %zu\n", bst_size(tree), bst_height(tree));
    bst_destroy(tree);
    return 0;
}
