#include "test_headers.h"
#include "utils.h"
#include "image.h"
#include "rect.h"
#include "curses.h"
#include "unit_images.h"
#include "units.h"
#include "gameobject.h"

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
    image1D.free(&image1D);

    Image image1D_2;
    const char *testString = "test";
    size_t lenTestString = strlen(testString);
    Image_init_1D(&image1D_2, lenTestString, testString);
    assert_int_equal(image1D_2.get_height(&image1D_2), 1);
    assert_int_equal(image1D_2.get_width(&image1D_2), lenTestString);
    assert_memory_equal(image1D_2.get_pixels(&image1D_2)[0], testString, lenTestString);
    image1D_2.free(&image1D_2);
    assert_int_equal(image1D_2.get_width(&image1D_2), 0);
    assert_int_equal(image1D_2.get_height(&image1D_2), 0);
    assert_null(image1D_2.get_pixels(&image1D_2));
}

static void test_Image_2D(void **state) {
    (void)state; /* Remove unused complaint. */

    Image image2D;
    const char **image1 = {""};
    Image_init_2D(&image2D, 0, 1, image1);
    assert_int_equal(image2D.get_height(&image2D), 1);
    assert_int_equal(image2D.get_width(&image2D), 0);
    assert_int_equal(image2D.get_pixels(&image2D)[0][0], '\0');
    image2D.free(&image2D);

    Image image2D_2;
    const char *testString[] = {"test"};
    size_t lenTestString = strlen("test");
    Image_init_2D(&image2D_2, lenTestString, 1, testString);
    assert_int_equal(image2D_2.get_height(&image2D_2), 1);
    assert_int_equal(image2D_2.get_width(&image2D_2), lenTestString);
    assert_string_equal(image2D_2.get_pixels(&image2D_2)[0], testString[0]);
    image2D_2.free(&image2D_2);
    assert_int_equal(image2D_2.get_width(&image2D_2), 0);
    assert_int_equal(image2D_2.get_height(&image2D_2), 0);
    assert_null(image2D_2.get_pixels(&image2D_2));

    Image image2D_3;
    const char *testString3[] = {"AAA", "BBB", "CCC"};
    size_t lenTestString3 = strlen(testString3[0]);
    Image_init_2D(&image2D_3, lenTestString3, lenTestString3, testString3);
    assert_memory_equal(image2D_3.get_pixels(&image2D_3)[0], "AAA", 3);
    assert_memory_equal(image2D_3.get_pixels(&image2D_3)[1], "BBB", 3);
    assert_memory_equal(image2D_3.get_pixels(&image2D_3)[2], "CCC", 3);
    assert_int_equal(image2D_3.get_height(&image2D_3), lenTestString3);
    assert_int_equal(image2D_3.get_width(&image2D_3), lenTestString3);
    assert_string_equal(image2D_3.get_pixels(&image2D_3)[0], testString3[0]);
    image2D_3.free(&image2D_3);
    image2D_3.free(&image2D_3);
    image2D_3.free(&image2D_3);
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
    char **pixels = storedImage->get_pixels(storedImage);
    assert_non_null(pixels);
    assert_string_equal(storedImage->get_pixels(storedImage)[0], "AA AA");

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
    Rect_init(&rect2, 5, 7, -1, -1);
    assert_int_equal(rect2.x, 5);
    assert_int_equal(rect2.y, 7);
    assert_int_equal(rect2.width, 0);
    assert_int_equal(rect2.height, 0);
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

static void test_Image_units(void **state) {
    (void)state;

    Image archer;
    Image_init_2D(&archer, UNIT_IMAGE_WIDTH, UNIT_IMAGE_HEIGHT, UNIT_STRING_ARCHER);
    assert_int_equal(archer.get_height(&archer), UNIT_IMAGE_HEIGHT);
    assert_int_equal(archer.get_width(&archer), UNIT_IMAGE_WIDTH);
    char **pixels_archer = archer.get_pixels(&archer);
    assert_string_equal(pixels_archer[0], UNIT_STRING_ARCHER[0]);
    assert_string_equal(pixels_archer[1], UNIT_STRING_ARCHER[1]);
    assert_string_equal(pixels_archer[2], UNIT_STRING_ARCHER[2]);

    Image swordman;
    Image_init_2D(&swordman, UNIT_IMAGE_WIDTH, UNIT_IMAGE_HEIGHT, UNIT_STRING_SWORDMAN);
    assert_int_equal(swordman.get_height(&swordman), UNIT_IMAGE_HEIGHT);
    assert_int_equal(swordman.get_width(&swordman), UNIT_IMAGE_WIDTH);
    char **pixels_swordman = swordman.get_pixels(&swordman);
    assert_string_equal(pixels_swordman[0], UNIT_STRING_SWORDMAN[0]);
    assert_string_equal(pixels_swordman[1], UNIT_STRING_SWORDMAN[1]);
    assert_string_equal(pixels_swordman[2], UNIT_STRING_SWORDMAN[2]);
}

static void test_unit_images(void **state) {
    (void)state;

    Image *archer = &UNIT_IMAGE_ARCHER;
    assert_int_equal(archer->get_height(archer), UNIT_IMAGE_HEIGHT);
    assert_int_equal(archer->get_width(archer), UNIT_IMAGE_WIDTH);
    assert_non_null(archer->get_pixels(archer));
    //assert_memory_equal(archer->get_pixels(archer), UNIT_STRING_ARCHER, sizeof(UNIT_IMAGE_ARCHER));
}

static void test_Units_constructors(void **state) {
    (void)state;
/*
    GameObject objects[3];
    GameObject_init(&objects[0], SURFACE_ARCHER, 0, 0);*/
}


/* ----- | Other | ------ */



int main(void) {
    UnitImages_init();

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_Image_1D),
        cmocka_unit_test(test_Image_2D),

        cmocka_unit_test(test_Curses_init),
        cmocka_unit_test(test_Surface_1D),
        cmocka_unit_test(test_Surface_2D),

        cmocka_unit_test(test_Rect_init),
        cmocka_unit_test(test_Rect_move),

        cmocka_unit_test(test_Image_units),
        cmocka_unit_test(test_unit_images),

        cmocka_unit_test(test_Units_constructors),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}