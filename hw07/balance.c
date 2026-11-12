#include "istack.h"
#include <stdio.h>
#include <stdlib.h>
static int opening(int c) { return c == '(' || c == '[' || c == '{'; }
static int closing(int c) { return c == ')' || c == ']' || c == '}'; }
static int expected(int c) { return c == '(' ? ')' : c == '[' ? ']' : '}'; }
int main(void) {
    istack_t *stack = istack_create();
    if (!stack) { fprintf(stderr, "out of memory\n"); return 1; }
    size_t line = 1, column = 0, pairs = 0, deepest = 0;
    int ch, top;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') { ++line; column = 0; } else ++column;
        if (opening(ch)) {
            if (istack_push(stack, ch) != 0) {
                istack_destroy(stack);
                fprintf(stderr, "out of memory\n");
                return 1;
            }
            if (istack_count(stack) > deepest) deepest = istack_count(stack);
        } else if (closing(ch)) {
            if (istack_is_empty(stack)) {
                printf("line %zu, column %zu: found '%c', nothing open\n", line, column, ch);
                istack_destroy(stack);
                return 1;
            }
            (void)istack_peek(stack, &top);
            if (expected(top) != ch) {
                printf("line %zu, column %zu: found '%c', expected '%c'\n", line, column, ch, expected(top));
                istack_destroy(stack);
                return 1;
            }
            (void)istack_pop(stack, NULL);
            ++pairs;
        }
    }
    if (!istack_is_empty(stack)) {
        (void)istack_peek(stack, &top);
        printf("end of input: %zu still open, innermost '%c'\n", istack_count(stack), top);
        istack_destroy(stack);
        return 1;
    }
    printf("balanced: %zu pairs, deepest %zu\n", pairs, deepest);
    istack_destroy(stack);
    return 0;
}
