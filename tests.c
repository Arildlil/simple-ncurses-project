#include "test_headers.h"
#include "utils.h"

#include "curses.h"
#include <curses.h>



/* ----- | Curses | ----- */

static void test_Curses_init(void **state) {
    assert_int_equal(Curses_is_inited(), FALSE);
    assert_int_equal(Curses_init(), TRUE);
    assert_int_equal(Curses_is_inited(), TRUE);
    assert_int_equal(Curses_exit(), TRUE);
}

static void test_Surface_init(void **state) {
    Surface surfaces[2];
    Surface_init(&surfaces[0], 0, 0, "", NULL);
    assert_int_equal(surfaces[0].x, 0);
    assert_int_equal(surfaces[0].y, 0);
    assert_int_equal(surfaces[0].content, "");
    assert_int_equal(surfaces[0].options.bounce, FALSE);

    Surface_Options options = {.bounce = TRUE};
    Surface_init(&surfaces[1], 10, 10, "Hello", &options);
    assert_int_equal(surfaces[1].x, 10);
    assert_int_equal(surfaces[1].y, 10);
    assert_int_equal(surfaces[1].content, "Hello");
    assert_int_equal(surfaces[1].options.bounce, TRUE);
}

static void test_Surface_move(void **state) {
    Surface surfaces[2]; 
    Surface_init(&surfaces[0], 5, 2, "o", NULL);
    Surface_move(&surfaces[0], 5, 5);
    assert_int_equal(surfaces[0].x, 10);
    assert_int_equal(surfaces[0].y, 7);
    
    Surface_Options options = {.bounce = TRUE};
    Surface_init(&surfaces[1], 30, 30, "o", &options);
    Surface_move(&surfaces[1], max_x+1, max_y+1);
    assert_int_equal(surfaces[1].x, max_x-1);
    assert_int_equal(surfaces[1].y, max_y-1);
}



/* ----- | Other | ------ */

static void null_test_success(void **state) {
    (void) state; /* Unused */
}

int main(void) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_Curses_init),
        cmocka_unit_test(test_Surface_init),
        cmocka_unit_test(test_Surface_move),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}