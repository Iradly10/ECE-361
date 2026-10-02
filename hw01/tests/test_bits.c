#include <stdio.h>
#include <stdint.h>

#include "../bits.h"
#include "../status.h"

static int tests_run = 0;
static int tests_failed = 0;

void check_test(const char *name, int condition)
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
    /* get_field tests */
    check_test("get_field width 1",
               get_field(0x1, 0, 1) == 1);

    check_test("get_field position 31",
               get_field(0x80000000u, 31, 1) == 1);

    check_test("get_field width 32",
               get_field(0x12345678u, 0, 32) == 0x12345678u);

    /* set_field tests */
    check_test("set_field basic",
               set_field(0x00u, 4, 3, 5) == 0x50u);

    check_test("set_field value too wide",
               set_field(0x00u, 4, 3, 0xFu) == 0x70u);

    /* sign_extend tests */
    check_test("sign_extend positive",
               sign_extend(0x7F, 8) == 127);

    check_test("sign_extend negative",
               sign_extend(0xF8, 8) == -8);

    check_test("sign_extend most negative",
               sign_extend(0x80, 8) == -128);

    check_test("sign_extend width 32",
               sign_extend(0xFFFFFFFFu, 32) == -1);

    /* status_unpack example from homework */
    status_t status = status_unpack(0x1631);

    check_test("status setpoint",
               status.setpoint == 22);

    check_test("status mode AUTO",
               status.mode == 3);

    check_test("status heater on",
               status.heat == 1);

    check_test("status compressor off",
               status.cool == 0);

    check_test("status fan off",
               status.fan == 0);

    check_test("status no fault",
               status.fault == 0);

    check_test("status reserved clear",
               status.reserved == 0);
    
    /* Second status word: negative setpoint */
status_t status2 = status_unpack(0xF802);

check_test("status2 setpoint -8",
           status2.setpoint == -8);

check_test("status2 mode OFF",
           status2.mode == 0);

check_test("status2 compressor on",
           status2.cool == 1);


/* Third status word: invalid mode */
status_t status3 = status_unpack(0x0050);

check_test("status3 invalid mode",
           status3.mode == 5);

    printf("\n%d tests run, %d failed\n",
           tests_run, tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
