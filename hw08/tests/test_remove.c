#include "../bst.h"
#include <stdio.h>
static int failures = 0;
#define CHECK(name, expr) do { if (expr) printf("PASS %s\n", name); else { printf("FAIL %s\n", name); ++failures; } } while (0)
int main(void) {
    bst_t *t = bst_create();
    if (!t) return 1;
    const int a[] = {27,14,35,10,19,31,42};
    for (size_t i = 0; i < 7; ++i) if (bst_insert(t,a[i])) return 1;
    CHECK("remove leaf 10", bst_remove(t,10)==0 && bst_size(t)==6 && !bst_contains(t,10,NULL));
    CHECK("remove one-child 14", bst_remove(t,14)==0 && bst_size(t)==5 && bst_contains(t,19,NULL));
    CHECK("remove two-child 27", bst_remove(t,27)==0 && bst_size(t)==4 && !bst_contains(t,27,NULL) && bst_contains(t,31,NULL));
    CHECK("missing key unchanged", bst_remove(t,999)==-1 && bst_size(t)==4);
    CHECK("NULL tree rejected", bst_remove(NULL,5)==-1);
    bst_destroy(t);
    t=bst_create(); if (!t) return 1;
    bst_insert(t,5); bst_insert(t,5); bst_insert(t,5);
    CHECK("remove one duplicate", bst_remove(t,5)==0 && bst_size(t)==2 && bst_contains(t,5,NULL));
    CHECK("remove remaining duplicates", bst_remove(t,5)==0 && bst_remove(t,5)==0 && bst_size(t)==0 && bst_height(t)==0);
    CHECK("remove empty tree", bst_remove(t,5)==-1);
    bst_destroy(t);
    printf("%s: %d failure(s)\n", failures ? "FAIL" : "PASS", failures);
    return failures ? 1 : 0;
}
