#include "../istack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
static int passed, failed;
#define CHECK(name, condition) do { if (condition) { printf("PASS %s\n", name); ++passed; } else { printf("FAIL %s\n", name); ++failed; } } while (0)
int main(void) {
    int v = 777;
    CHECK("NULL count", istack_count(NULL) == 0);
    CHECK("NULL empty", istack_is_empty(NULL));
    CHECK("NULL capacity", istack_capacity(NULL) == 0);
    CHECK("NULL push", istack_push(NULL, 4) == -1);
    CHECK("NULL pop", istack_pop(NULL, &v) == -1 && v == 777);
    CHECK("NULL peek", istack_peek(NULL, &v) == -1 && v == 777);
    size_t n = 999;
    CHECK("NULL to_array", istack_to_array(NULL, &n) == NULL && n == 0);
    istack_destroy(NULL);
    CHECK("NULL destroy", true);
    istack_t *s = istack_create();
    if (!s) { puts("FAIL create (out of memory)"); return 1; }
    CHECK("initial empty", istack_is_empty(s) && istack_count(s) == 0);
    CHECK("initial capacity", istack_capacity(s) == 8);
    CHECK("empty pop preserves output", istack_pop(s, &v) == -1 && v == 777);
    CHECK("empty peek preserves output", istack_peek(s, &v) == -1 && v == 777);
    CHECK("empty array", istack_to_array(s, &n) == NULL && n == 0);
    for (int i = 0; i < 8; ++i) if (istack_push(s, i) != 0) { puts("FAIL push"); return 1; }
    CHECK("eight values", istack_count(s) == 8 && istack_capacity(s) == 8);
    CHECK("ninth doubles", istack_push(s, 8) == 0 && istack_capacity(s) == 16);
    CHECK("peek ninth", istack_peek(s, &v) == 0 && v == 8);
    CHECK("pop ninth", istack_pop(s, &v) == 0 && v == 8);
    CHECK("capacity does not shrink", istack_capacity(s) == 16);
    CHECK("pop NULL output", istack_pop(s, NULL) == 0 && istack_count(s) == 7);
    CHECK("peek NULL output", istack_peek(s, NULL) == 0 && istack_count(s) == 7);
    int *a = istack_to_array(s, &n);
    CHECK("array bottom first", a && n == 7 && a[0] == 0 && a[6] == 6);
    if (a) { a[0] = 999; free(a); }
    CHECK("array is independent", istack_peek(s, &v) == 0 && v == 6);
    istack_t *other = istack_create();
    CHECK("second stack", other && istack_push(other, 123) == 0 && istack_count(s) == 7);
    istack_destroy(other);
    istack_destroy(s);
    s = istack_create();
    if (!s) return 1;
    bool all = true;
    for (int i = 0; i < 1000; ++i) if (istack_push(s, i) != 0) all = false;
    CHECK("1000 pushes", all && istack_count(s) == 1000 && istack_capacity(s) >= 1000);
    for (int i = 999; i >= 0; --i) if (istack_pop(s, &v) != 0 || v != i) all = false;
    CHECK("1000 pops in reverse order", all && istack_is_empty(s));
    CHECK("room retained", istack_capacity(s) >= 1000);
    istack_destroy(s);
    s = istack_create();
    CHECK("destroy nonempty", s && istack_push(s, 42) == 0);
    istack_destroy(s);
    printf("Summary: %d passed, %d failed\n", passed, failed);
    return failed ? 1 : 0;
}
