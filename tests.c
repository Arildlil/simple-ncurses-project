#include "test_headers.h"
#include "utils.h"
#include "image.h"
#include "rect.h"
#include "curses.h"
#include "unit_images.h"
#include "unit_surfaces.h"
#include "gameobject.h"
#include "units.h"
#include "player_controls.h"
#include "player.h"
#include "orders_utils.h"

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

static void test_unit_surfaces(void **state) {
    (void)state;

    Surface surfs[1];
    UnitSurface_archer(&surfs[0], 1, 2);
    
    Image *image_archer = surfs[0].get_image(&surfs[0]);
    assert_int_equal(image_archer->get_width(image_archer), UNIT_IMAGE_ARCHER.get_width(&UNIT_IMAGE_ARCHER));
    assert_int_equal(image_archer->get_height(image_archer), UNIT_IMAGE_ARCHER.get_height(&UNIT_IMAGE_ARCHER));
    
    int image_width = image_archer->get_width(image_archer);
    char **pixels = image_archer->get_pixels(image_archer);
    char **pixels_original = UNIT_IMAGE_ARCHER.get_pixels(&UNIT_IMAGE_ARCHER);
    assert_memory_equal(pixels[0], pixels_original[0], sizeof(char) * image_width);
    assert_memory_equal(pixels[1], pixels_original[1], sizeof(char) * image_width);
    assert_memory_equal(pixels[2], pixels_original[2], sizeof(char) * image_width);
}

static void test_Units_constructors(void **state) {
    (void)state;

    //GameObject objects[3];
    //GameObject_init(&objects[0], SURFACE_ARCHER, 0, 0);
}

static void test_Gameobject(void **state) {
    (void)state;

    GameObject object;
    GameObject_init(&object, &dummy_player, 3, 5, &UNIT_IMAGE_ARCHER, NULL);
    assert_int_equal(object.m->is_active(&object), TRUE);

    assert_ptr_equal(object.m->get_owner(&object), &dummy_player);
    assert_int_equal(object.m->get_x(&object), 3);
    assert_int_equal(object.m->get_y(&object), 5);
    object.m->movement(&object, 2, 3);
    assert_int_equal(object.m->get_x(&object), 5);
    assert_int_equal(object.m->get_y(&object), 8);
    object.m->set_x(&object, 2);
    object.m->set_y(&object, 1);
    assert_int_equal(object.m->get_x(&object), 2);
    assert_int_equal(object.m->get_y(&object), 1);
    object.m->set_xy(&object, 3, 4);
    assert_int_equal(object.m->get_x(&object), 3);
    assert_int_equal(object.m->get_y(&object), 4);

    assert_memory_equal(object.m->get_image(&object), &UNIT_IMAGE_ARCHER, sizeof(Image));
    assert_int_equal(object.m->get_width(&object), UNIT_IMAGE_ARCHER.get_width(&UNIT_IMAGE_ARCHER));
    assert_int_equal(object.m->get_height(&object), UNIT_IMAGE_ARCHER.get_height(&UNIT_IMAGE_ARCHER));
    int image_width = object.m->get_width(&object);
    char **pixels = object.m->get_pixels(&object);
    char **pixels_original = UNIT_IMAGE_ARCHER.get_pixels(&UNIT_IMAGE_ARCHER);
    assert_memory_equal(pixels[0], pixels_original[0], sizeof(char) * image_width);

    object.m->free(&object);
    assert_int_equal(object.m->is_active(&object), FALSE);

    GameObject_init(NULL, &dummy_player, 2, 4, &UNIT_IMAGE_ARCHER, NULL);
}

static void test_Player_controls(void **state) {
    (void)state;

    GameObject object;
    Units_init_archer(&object, &dummy_player, 5, 6);
    #define NUM_TO_CHECK 8
    char chars_to_check[NUM_TO_CHECK] = {'a', 'A', 'w', 'W', 's', 'S', 'd', 'D'};
    int i;

    assert_int_equal(PlayerControls_handle_input_char('a', NULL), FALSE);
    
    for (i = 0; i < NUM_TO_CHECK; i++) { 
        assert_int_equal(PlayerControls_handle_input_char(chars_to_check[i], &object), TRUE);
    }
}

static void test_Player(void **state) {
    (void)state;

    Player players[2];
    Player_init(&players[0], COLOR_PAIR_RED, TRUE);
    Player_init(&players[1], COLOR_PAIR_RED, FALSE);

    assert_in_range(players[0].m->get_player_id(&players[0]), 0, 10);
    assert_int_equal(players[0].m->get_colors(&players[0]), COLOR_PAIR_RED);
    assert_int_equal(players[0].m->is_real_player(&players[0]), TRUE);
}

static void dummy_on_tick(GameObject_Controller *controller, GameObject *object) {
    object->m->movement(object, 1, 1);
}
static void test_GameObject_Controller(void **state) {
    (void)state;

    GameObject object;
    Units_init_archer(&object, &dummy_player, 0, 0);
    GameObject_Controller controller;
    GameObject_Controller_init(&controller, NULL);
    assert_non_null(controller.m);
    memset(&controller, 0, sizeof(controller));

    GameObject_Controller_Methods methods = {.on_tick = dummy_on_tick};
    GameObject_Controller_init(&controller, &methods);
    assert_non_null(controller.m);
    object.m->set_controller(&object, &controller);
    int old_x = object.m->get_x(&object);
    int old_y = object.m->get_y(&object);
    GameObject_Controller *retrieved_controller = object.m->get_controller(&object);
    retrieved_controller->m->on_tick(&retrieved_controller, &object);
    assert_int_equal(object.m->get_x(&object), old_x + 1);
    assert_int_equal(object.m->get_y(&object), old_y + 1);
}

static void test_Order(void **state) {
    (void)state;

    GameObject object;
    Units_init_archer(&object, &dummy_player, 0, 0);

    Order order;
    Orders_move(&order, 4, 6);
    assert_int_equal(order.is_active, TRUE);
    assert_int_equal(order.type, ORDER_TYPE_MOVE);
    assert_int_equal(order.destination.desttype, DEST_TYPE_COORDINATES);
    assert_int_equal(order.destination.coordinates.x, 4);
    assert_int_equal(order.destination.coordinates.y, 6);

    memset(&order, 0, sizeof(Order));
    Orders_attack(&order, &object);
    assert_int_equal(order.is_active, TRUE);
    assert_int_equal(order.type, ORDER_TYPE_ATTACK);
    assert_int_equal(order.destination.desttype, DEST_TYPE_GAMEOBJECT);
    assert_ptr_equal(order.destination.object, &object);
}

static void test_GameObject_Order(void **state) {
    (void)state;

    GameObject object;
    GameObject object2;
    Units_init_archer(&object, &dummy_player, 5, 10);
    Units_init_archer(&object2, &dummy_player, 10, 5);
    assert_non_null(object.m->get_current_order(&object));
    assert_int_equal(object.m->get_current_order(&object)->type, ORDER_TYPE_NONE);
    assert_int_equal(object.m->get_order_count(&object), 0);
    object.m->move_to(&object, 10, 5, FALSE);
    assert_int_equal(object.m->get_order_count(&object), 1);
    object.m->attack(&object, &object2, FALSE);
    assert_int_equal(object.m->get_order_count(&object), 1);
    object.m->move_to(&object, 15, 2, TRUE);
    assert_int_equal(object.m->get_order_count(&object), 2);
    object.m->attack(&object, &object2, TRUE);
    assert_int_equal(object.m->get_order_count(&object), 3);
    Order *current_order = object.m->get_current_order(&object);
    assert_non_null(current_order);
    assert_int_equal(current_order->type, ORDER_TYPE_ATTACK);
    object.m->move_to(&object, 15, 2, FALSE);
    assert_int_equal(object.m->get_order_count(&object), 1);
    current_order = object.m->get_current_order(&object);
    assert_non_null(current_order);
    assert_int_equal(current_order->type, ORDER_TYPE_MOVE);
    assert_int_equal(current_order->destination.coordinates.x, 15);
    assert_int_equal(current_order->destination.coordinates.y, 2);
}

static void test_GameObject_Order_queue(void **state) {
    (void)state;
    
    GameObject object;
    Units_init_archer(&object, &dummy_player, 5, 5);
    
    Order order1, order2, order3;
    Orders_move(&order1, 10, 15);
    Orders_move(&order2, 7, 8);
    Orders_move(&order3, 2, 2);
    assert_int_equal(insert_order(&object, &order3, FALSE), TRUE);
    assert_int_equal(object.m->get_order_count(&object), 1);

    assert_int_equal(insert_order(&object, &order1, FALSE), TRUE);
    assert_int_equal(object.m->get_order_count(&object), 1);
    assert_int_equal(insert_order(&object, &order2, TRUE), TRUE);
    assert_int_equal(object.m->get_order_count(&object), 2);
    assert_int_equal(insert_order(&object, &order3, TRUE), TRUE);
    assert_int_equal(object.current_order_index, 0);
    assert_int_equal(object.m->get_order_count(&object), 3);
    remove_current_order(&object);
    assert_int_equal(object.current_order_index, 1);
    assert_int_equal(object.m->get_order_count(&object), 2);

    Order *current_order = object.m->get_current_order(&object);
    assert_int_equal(current_order->destination.coordinates.x, order2.destination.coordinates.x);
    assert_int_equal(current_order->destination.coordinates.y, order2.destination.coordinates.y);

    clear_order_queue(&object);
    assert_int_equal(object.m->get_order_count(&object), 0);
    assert_int_equal(object.current_order_index, 0);
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
        cmocka_unit_test(test_unit_surfaces),
        cmocka_unit_test(test_Units_constructors),

        cmocka_unit_test(test_Gameobject),
        cmocka_unit_test(test_Player_controls),
        cmocka_unit_test(test_Player),

        cmocka_unit_test(test_GameObject_Controller),
        cmocka_unit_test(test_Order),
        cmocka_unit_test(test_GameObject_Order),
        cmocka_unit_test(test_GameObject_Order_queue),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}