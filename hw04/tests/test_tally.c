#include <stdio.h>
#include <string.h>
#include "../tally.h"

static int tests_run = 0;
static int tests_failed = 0;
static void check_test(const char *name, int condition)
{
    tests_run++;

    if (condition)
    {
        printf("PASS: %s\n", name);
    }
    else
    {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}
int main(void)
{
    tally_t *t = tally_create();

    check_test("create tally", t != NULL);

    check_test("add door", tally_add(t, "door") == 1);
    check_test("count door", tally_count(t, "door") == 1);
    check_test("add door again", tally_add(t, "door") == 2);
    check_test("count door again", tally_count(t, "door") == 2);
    check_test("size after door", tally_size(t) == 1);
    check_test("add window", tally_add(t, "window") == 1);
    check_test("size after window", tally_size(t) == 2);
    check_test("reject empty name", tally_add(t, "") == -1);
    check_test("reject NULL name", tally_add(t, NULL) == -1);
    check_test("reject NULL tally", tally_add(NULL, "door") == -1);
    check_test("count missing name", tally_count(t, "apple") == 0);
    check_test("size NULL tally", tally_size(NULL) == 0);
    check_test("accept 15 character name",
               tally_add(t, "abcdefghijklmno") == 1);

    check_test("reject 16 character name",
               tally_add(t, "abcdefghijklmnop") == -1);
    tally_t *full = tally_create();

    for (int i = 0; i < TALLY_NAMES; i++)
    {
        char name[16];
        snprintf(name, sizeof(name), "name%d", i);
        tally_add(full, name);
    }

    check_test("full tally has 16 names",
               tally_size(full) == TALLY_NAMES);

    check_test("reject 17th name",
               tally_add(full, "extra") == -1);
    tally_t *second = tally_create();
    tally_t *third = tally_create();

    check_test("create four tallies",
               t != NULL && full != NULL &&
               second != NULL && third != NULL);

    tally_t *fifth = tally_create();

    check_test("reject fifth tally", fifth == NULL);

    tally_destroy(second);
    tally_t *reused = tally_create();

    check_test("reuse destroyed tally", reused != NULL);
    check_test("reused tally starts empty", tally_size(reused) == 0);
    check_test("add to reused tally",
               tally_add(reused, "apple") == 1);

    check_test("reused tally count",
               tally_count(reused, "apple") == 1);

    check_test("other tally stays independent",
               tally_count(t, "apple") == 0);
    FILE *out = tmpfile();

    check_test("create temporary output", out != NULL);

    if (out != NULL)
    {
        tally_print(t, out);
        rewind(out);

        char first_line[64];
        fgets(first_line, sizeof(first_line), out);

        check_test("printed names are sorted",
                   strcmp(first_line, "abcdefghijklmno 1\n") == 0);

        fclose(out);
    }

    tally_destroy(reused);
    tally_destroy(third);
    tally_destroy(full);
    tally_destroy(t);

    printf("Tests run: %d, failed: %d\n", tests_run, tests_failed);

    return tests_failed == 0 ? 0 : 1;
}