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

    Image image1D;
    Image_init_1D(&image1D, 1, "");
    assert_int_equal(image1D.getHeight(&image1D), 1);
    assert_int_equal(image1D.getWidth(&image1D), 1);
    assert_int_equal(image1D.getPixels(&image1D)[0][0], '\0');

    Image image1D_2;
    char *testString = "test";
    size_t lenTestString = strlen(testString);
    Image_init_1D(&image1D_2, lenTestString, testString);
    assert_int_equal(image1D_2.getHeight(&image1D_2), 1);
    assert_int_equal(image1D_2.getWidth(&image1D_2), lenTestString);
    assert_memory_equal(image1D_2.getPixels(&image1D_2)[0], testString, lenTestString);

    image1D_2.free(&image1D_2);
    assert_int_equal(image1D_2.getWidth(&image1D_2), 0);
    assert_int_equal(image1D_2.getHeight(&image1D_2), 0);
    // assert_int_equal(image1D_2.getPixels(), NULL);
}

static void test_Image_2D(void **state) {
    (void)state; /* Remove unused complaint. */

    Image image2D;
    char **image1 = {""};
    Image_init_2D(&image2D, 1, 1, image1);
    assert_int_equal(image2D.getHeight(&image2D), 1);
    assert_int_equal(image2D.getWidth(&image2D), 1);
    assert_int_equal(image2D.getPixels(&image2D)[0][0], '\0');

    Image image2D_2;
    char **testString = {"test"};
    size_t lenTestString = strlen(testString[0]);
    Image_init_2D(&image2D_2, lenTestString, 1, testString);
    assert_int_equal(image2D_2.getHeight(&image2D_2), 1);
    assert_int_equal(image2D_2.getWidth(&image2D_2), lenTestString);
    assert_memory_equal(image2D_2.getPixels(&image2D_2)[0], testString, lenTestString);

    Image image2D_3;
    char **testString3 = {"AAA", "BBB", "CCC"};
    size_t lenTestString3 = strlen(testString3[0]);
    Image_init_2D(&image2D_3, lenTestString3, lenTestString3, testString3);
    assert_memory_equal(image2D_3.getPixels(&image2D_3)[0], "AAA", 3);
    assert_memory_equal(image2D_3.getPixels(&image2D_3)[1], "BBB", 3);
    assert_memory_equal(image2D_3.getPixels(&image2D_3)[2], "CCC", 3);
    assert_int_equal(image2D_3.getHeight(&image2D_3), lenTestString3);
    assert_int_equal(image2D_3.getWidth(&image2D_3), lenTestString3);
    assert_memory_equal(image2D_3.getPixels(&image2D_3)[0], testString3, lenTestString3);

    image2D_2.free(&image2D_2);
    assert_int_equal(image2D_2.getWidth(&image2D_2), 0);
    assert_int_equal(image2D_2.getHeight(&image2D_2), 0);
    assert_null(image2D_2.getPixels(&image2D_2));
}

static void test_Surface_init(void **state) {
    (void)state; /* Remove unused complaint. */

    /*
    Surface surface1;
    char image1[1][1] = {""};
    Surface_init(&surface1, 0, 0, 0, 0, image1, NULL);
    assert_int_equal(surface1.rect.x, 0);
    assert_int_equal(surface1.rect.y, 0);
    // assert_int_equal(surface1.content[0][0], "");
    assert_int_equal(surface1.options.bounce, FALSE);

    Surface surface2;
    char image2[1][1] = {"Hello"};
    Surface_Options options = {.bounce = TRUE};
    Surface_init(&surface2, 10, 10, 5, 1, image2, &options);
    assert_int_equal(surface2.rect.x, 10);
    assert_int_equal(surface2.rect.y, 10);
    assert_int_equal(surface2.content, "Hello");
    assert_int_equal(surface2.options.bounce, TRUE);*/
}

static void test_Surface_move(void **state) {
    (void)state; /* Remove unused complaint. */

    /*
    Surface surfaces[2]; 
    char image1[1][1] = {"o"};
    Surface_init(&surfaces[0], 5, 2, 1, 1, image1, NULL);
    Surface_move(&surfaces[0], 5, 5);
    assert_int_equal(surfaces[0].rect.x, 10);
    assert_int_equal(surfaces[0].rect.y, 7);
    
    Surface_Options options = {.bounce = TRUE};
    char image2[1][1] = {"o"};
    Surface_init(&surfaces[1], 30, 30, 1, 1, image2, &options);
    Surface_move(&surfaces[1], max_x+1, max_y+1);
    assert_int_equal(surfaces[1].rect.x, max_x-1);
    assert_int_equal(surfaces[1].rect.y, max_y-1);*/
}

static void test_Surface_remove(void **state) {
    (void)state; /* Remove unused complaint. */

    /*
    Surface surface;
    Surface_Options options = {.bounce = TRUE, .screen_only = FALSE};
    Surface_init(&surface, 10, 5, "o", &options);
    Surface_remove(&surface);
    assert_int_equal(surface.rect.x, 0);
    assert_int_equal(surface.rect.y, 0);
    assert_int_equal(surface.content, NULL);
    assert_int_equal(surface.state, DEAD);*/
}

/*
static void test_Surface_bounce(void **state) {
    
    Surface surface;
    Surface_Options options = {.bounce = TRUE, .screen_only = FALSE};
    Surface_init(&surface, max_x/2, max_y, "o", &options);
    Surface_bounce(&surface, max_x/2-1, max_y-1);
    assert_int_equal(surface.x, max_x+1);
    assert_int_equal(surface.y, max_y+1);
}*/

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

        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_Curses_init),
        cmocka_unit_test(test_Surface_init),
        cmocka_unit_test(test_Surface_move),
        cmocka_unit_test(test_Surface_remove),

        cmocka_unit_test(test_Rect_init),
        cmocka_unit_test(test_Rect_move),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}