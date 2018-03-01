#include "test_headers.h"
#include "utils.h"

#include "curses.h"



/* ----- | Curses | ----- */

static void test_Curses_init(void **state) {
    assert_int_equal(Curses_is_inited(), FALSE);
    assert_int_equal(Curses_init(), TRUE);
    assert_int_equal(Curses_is_inited(), TRUE);
}



/* ----- | Other | ------ */

static void null_test_success(void **state) {
    (void) state; /* Unused */
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_Curses_init),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}