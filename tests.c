#include "test_headers.h"
#include "utils.h"
#include "image.h"
#include "rect.h"
#include "curses.h"

#include <string.h>
#include <curses.h>



/* ----- | Curses | ----- */

static void test_Curses_init(void **state) {
    (void)state; /* Remove unused complaint. */

    assert_int_equal(Curses_is_inited(), FALSE);
    assert_int_equal(Curses_init(), TRUE);
    assert_int_equal(Curses_is_inited(), TRUE);
    assert_int_equal(Curses_exit(), TRUE);
}

static void test_Image_1D(void **state) {
    (void)state; /* Remove unused complaint. */

    size_t lenEmptyString = strlen("");
    Image image1D;
    Image_init_1D(&image1D, 0, "");
    assert_int_equal(image1D.get_height(&image1D), 1);
    assert_int_equal(image1D.get_width(&image1D), lenEmptyString);
    assert_int_equal(image1D.get_pixels(&image1D)[0][0], '\0');

    Image image1D_2;
    char *testString = "test";
    size_t lenTestString = strlen(testString);
    Image_init_1D(&image1D_2, lenTestString, testString);
    assert_int_equal(image1D_2.get_height(&image1D_2), 1);
    assert_int_equal(image1D_2.get_width(&image1D_2), lenTestString);
    assert_memory_equal(image1D_2.get_pixels(&image1D_2)[0], testString, lenTestString);

    image1D_2.free(&image1D_2);
    assert_int_equal(image1D_2.get_width(&image1D_2), 0);
    assert_int_equal(image1D_2.get_height(&image1D_2), 0);
    // assert_int_equal(image1D_2.get_pixels(), NULL);
}

static void test_Image_2D(void **state) {
    (void)state; /* Remove unused complaint. */

    Image image2D;
    char **image1 = {""};
    Image_init_2D(&image2D, 0, 1, image1);
    assert_int_equal(image2D.get_height(&image2D), 1);
    assert_int_equal(image2D.get_width(&image2D), 0);
    assert_int_equal(image2D.get_pixels(&image2D)[0][0], '\0');

    Image image2D_2;
    char **testString = {"test"};
    size_t lenTestString = strlen(testString[0]);
    Image_init_2D(&image2D_2, lenTestString, 1, testString);
    assert_int_equal(image2D_2.get_height(&image2D_2), 1);
    assert_int_equal(image2D_2.get_width(&image2D_2), lenTestString);
    assert_memory_equal(image2D_2.get_pixels(&image2D_2)[0], testString, lenTestString);

    Image image2D_3;
    char **testString3 = {"AAA", "BBB", "CCC"};
    size_t lenTestString3 = strlen(testString3[0]);
    Image_init_2D(&image2D_3, lenTestString3, lenTestString3, testString3);
    assert_memory_equal(image2D_3.get_pixels(&image2D_3)[0], "AAA", 3);
    assert_memory_equal(image2D_3.get_pixels(&image2D_3)[1], "BBB", 3);
    assert_memory_equal(image2D_3.get_pixels(&image2D_3)[2], "CCC", 3);
    assert_int_equal(image2D_3.get_height(&image2D_3), lenTestString3);
    assert_int_equal(image2D_3.get_width(&image2D_3), lenTestString3);
    assert_memory_equal(image2D_3.get_pixels(&image2D_3)[0], testString3, lenTestString3);

    image2D_2.free(&image2D_2);
    assert_int_equal(image2D_2.get_width(&image2D_2), 0);
    assert_int_equal(image2D_2.get_height(&image2D_2), 0);
    assert_null(image2D_2.get_pixels(&image2D_2));
}

static void test_Surface_1D(void **state) {
    (void)state; /* Remove unused complaint. */

    /* Checks that it doesn't crash. */
    Surface_init_char(NULL, '*', 5, 3, NULL);

    /* Normal tests. */
    Surface surface1D;
    Surface_init_char(&surface1D, '*', 5, 3, NULL);

    assert_int_equal(surface1D.get_x(&surface1D), 5);
    assert_int_equal(surface1D.get_y(&surface1D), 3);
    assert_non_null(surface1D.get_image(&surface1D));
    surface1D.movement(&surface1D, 2, 3);
    assert_int_equal(surface1D.get_x(&surface1D), 7);
    assert_int_equal(surface1D.get_y(&surface1D), 6);
    assert_int_equal(surface1D.get_state(&surface1D), ALIVE);
    surface1D.free(&surface1D);
    assert_int_equal(surface1D.get_state(&surface1D), DEAD);
    assert_null(surface1D.get_image(&surface1D));
}

static void test_Surface_2D(void **state) {
    (void)state; /* Remove unused complaint. */

    Surface surface2D;
    Image image2D;
    Image_init_2D(&image2D, 5, 2, (char*[]){"AA AA", "BBBBBB"});
    Surface_init_image(&surface2D, &image2D, 9, 2, 
        &(Surface_Options){.screen_only = TRUE});

    assert_int_equal(surface2D.get_x(&surface2D), 9);
    assert_int_equal(surface2D.get_y(&surface2D), 2);
    assert_non_null(surface2D.get_image(&surface2D));
    surface2D.movement(&surface2D, 2, 3);
    assert_int_equal(surface2D.get_x(&surface2D), 11);
    assert_int_equal(surface2D.get_y(&surface2D), 5);
    assert_int_equal(surface2D.get_state(&surface2D), ALIVE);

    Image *storedImage = surface2D.get_image(&surface2D);
    assert_non_null(storedImage);
    assert_memory_equal(storedImage->get_pixels(storedImage)[0], (char*)("AA AA"), 5);

    surface2D.free(&surface2D);
    assert_int_equal(surface2D.get_state(&surface2D), DEAD);
    assert_null(surface2D.get_image(&surface2D));
}

static void test_Rect_init(void **state) {
    (void)state; /* Remove unused complaint. */

    Rect rect;
    Rect_init(&rect, 0, 0, 1, 1);
    assert_int_equal(rect.x, 0);
    assert_int_equal(rect.y, 0);
    assert_int_equal(rect.width, 1);
    assert_int_equal(rect.height, 1);

    Rect rect2;
    Rect_init(&rect, 5, 7, -1, -1);
    assert_int_equal(rect.x, 5);
    assert_int_equal(rect.y, 7);
    assert_int_equal(rect.width, 0);
    assert_int_equal(rect.height, 0);
}

static void test_Rect_move(void **state) {
    (void)state; /* Remove unused complaint. */
    
    Rect rect;
    Rect_init(&rect, 4, 3, 1, 1);
    Rect_move(&rect, 5, 5);
    assert_int_equal(rect.x, 9);
    assert_int_equal(rect.y, 8);
    assert_int_equal(rect.width, 1);
    assert_int_equal(rect.height, 1);
}


/* ----- | Other | ------ */

static void null_test_success(void **state) {
    (void) state; /* Unused */
}

int main(void) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_Image_1D),
        cmocka_unit_test(test_Image_1D),

        cmocka_unit_test(test_Curses_init),
        cmocka_unit_test(test_Surface_1D),
        cmocka_unit_test(test_Surface_2D),

        cmocka_unit_test(test_Rect_init),
        cmocka_unit_test(test_Rect_move),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}