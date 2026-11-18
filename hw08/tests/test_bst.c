#include "../bst.h"
#include <stdio.h>

static int failures = 0;
#define CHECK(name, condition) do { \
    if (condition) printf("PASS %s\n", name); \
    else { printf("FAIL %s\n", name); ++failures; } \
} while (0)

int main(void) {
    size_t visits = 999;
    CHECK("NULL tree contains", !bst_contains(NULL, 42, &visits) && visits == 0);
    CHECK("NULL tree size and height", bst_size(NULL) == 0 && bst_height(NULL) == 0);
    CHECK("NULL insert rejected", bst_insert(NULL, 1) == -1);
    bst_destroy(NULL);

    bst_t *empty = bst_create();
    if (empty == NULL) return 1;
    visits = 999;
    /* Regression: original search dereferenced NULL before checking it. */
    CHECK("original crash: search empty tree", !bst_contains(empty, 31, &visits) && visits == 0);
    CHECK("empty tree measurements", bst_size(empty) == 0 && bst_height(empty) == 0);
    bst_destroy(empty);

    bst_t *t = bst_create();
    if (t == NULL) return 1;
    const int keys[] = {27, 14, 35, 10, 19, 31, 42};
    int ok = 1;
    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i)
        if (bst_insert(t, keys[i]) != 0) ok = 0;
    CHECK("seven inserts", ok);
    CHECK("seven keys size 7 height 3", bst_size(t) == 7 && bst_height(t) == 3);
    CHECK("31 found in three visits", bst_contains(t, 31, &visits) && visits == 3);
    CHECK("15 missing in three visits", !bst_contains(t, 15, &visits) && visits == 3);
    CHECK("root found in one visit", bst_contains(t, 27, &visits) && visits == 1);
    CHECK("NULL visits pointer", bst_contains(t, 42, NULL));
    bst_destroy(t);
    CHECK("destroy populated tree", 1);

    t = bst_create();
    if (t == NULL) return 1;
    CHECK("insert first duplicate", bst_insert(t, 30) == 0);
    CHECK("insert second duplicate", bst_insert(t, 30) == 0);
    CHECK("duplicate retained to right", bst_size(t) == 2 && bst_height(t) == 2);
    bst_destroy(t);

    t = bst_create();
    if (t == NULL) return 1;
    ok = 1;
    for (int i = 1; i <= 25; ++i)
        if (bst_insert(t, i) != 0) ok = 0;
    CHECK("increasing inserts", ok);
    CHECK("increasing order height equals size", bst_size(t) == 25 && bst_height(t) == 25);
    CHECK("deep search visits", bst_contains(t, 25, &visits) && visits == 25);
    bst_destroy(t);

    printf("%s: %d failure(s)\n", failures ? "FAIL" : "PASS", failures);
    return failures ? 1 : 0;
}
