#include "ht.h"
#include <stdio.h>
#include <stdint.h>

static int check(const char *name, int ok)
{
    printf("%s: %s\n", ok ? "PASS" : "FAIL", name);
    return !ok;
}

static int sequence(size_t buckets, int step, size_t longest)
{
    ht_t *t = ht_create(buckets);
    if (!t)
        return 0;
    int ok = 1;
    for (int i = 1; i <= 1000; ++i)
        if (ht_add(t, i * step) != 0)
            ok = 0;
    for (int i = 1; i <= 1000; ++i)
        if (ht_count(t, i * step) != 1)
            ok = 0;
    ok = ok && ht_size(t) == 1000 && ht_longest_chain(t) == longest;
    ht_destroy(t);
    return ok;
}

int main(void)
{
    int failures = 0;
    ht_t *t = ht_create(0);
    failures += check("zero buckets", t == NULL);
    ht_destroy(t);
    t = ht_create(SIZE_MAX);
    failures += check("oversized allocation rejected", t == NULL);
    ht_destroy(t);
    t = ht_create(10);
    failures += check("empty table", t && ht_size(t) == 0 &&
                      ht_count(t, 5) == 0 && ht_longest_chain(t) == 0);
    int ok = t != NULL;
    for (int i = 0; i < 3; ++i)
        if (ht_add(t, 5) != 0)
            ok = 0;
    failures += check("same key three times", ok && ht_count(t, 5) == 3 &&
                      ht_size(t) == 1 && ht_longest_chain(t) == 1);
    ok = ht_add(t, 15) == 0;
    failures += check("collision and missing key", ok && ht_count(t, 5) == 3 &&
                      ht_count(t, 15) == 1 && ht_count(t, 25) == 0 &&
                      ht_size(t) == 2 && ht_longest_chain(t) == 2);
    ht_destroy(t);
    failures += check("Table 2 row 1: 10 buckets, chain 100", sequence(10, 1, 100));
    failures += check("Table 2 row 2: 1000 buckets, chain 1", sequence(1000, 1, 1));
    failures += check("Table 2 row 3: 1000 buckets, chain 10", sequence(1000, 10, 10));
    failures += check("Table 2 row 4: 1009 buckets, chain 1", sequence(1009, 10, 1));
    failures += check("NULL arguments", ht_add(NULL, 1) == -1 &&
                      ht_count(NULL, 1) == 0 && ht_size(NULL) == 0 &&
                      ht_longest_chain(NULL) == 0);
    ht_destroy(NULL);
    t = ht_create(1);
    ok = ht_add(t, 0) == 0 && ht_add(t, 10) == 0;
    failures += check("zero key and one bucket", ok && ht_count(t, 0) == 1 &&
                      ht_size(t) == 2 && ht_longest_chain(t) == 2);
    ht_destroy(t);
    printf("%d failed\n", failures);
    return failures ? 1 : 0;
}
