#include "../../include/gameobject.h"
#include "../../unit_images.h"
#include "orders_utils.h"



/* ----- | Prototypes | ------ */

/* Utils */
static void GameObject_free(struct GameObject *object);
static boolean GameObject_is_active(struct GameObject *object);
static Player *GameObject_get_owner(struct GameObject *object);
static GameObject_Controller *GameObject_get_controller(struct GameObject *object);
static void GameObject_set_controller(struct GameObject *object, struct GameObject_Controller *controller);
static Direction GameObject_get_direction(struct GameObject *object);
static void GameObject_set_direction(struct GameObject *object, Direction direction);

/* Orders */
/*
static boolean insert_order(struct GameObject *object, Order *order, boolean queued);
static void remove_current_order(struct GameObject *object);
static void clear_order_queue(struct GameObject *object);
*/

static void GameObject_on_tick(struct GameObject *object);
static int GameObject_get_order_count(struct GameObject *object);
static Order *GameObject_get_current_order(struct GameObject *object);
static boolean GameObject_move_to(struct GameObject *object, int x, int y, boolean queued);
static boolean GameObject_attack(struct GameObject *object, struct GameObject *target, boolean queued);
static boolean GameObject_shoot(struct GameObject *object);

/* Movement and coordinates */
static void GameObject_movement(struct GameObject *object, int x, int y);
static int GameObject_get_x(struct GameObject *object);
static int GameObject_get_y(struct GameObject *object);
static void GameObject_set_x(struct GameObject *object, int x);
static void GameObject_set_y(struct GameObject *object, int y);
static void GameObject_set_xy(struct GameObject *object, int x, int y);
static Surface *GameObject_get_surface(struct GameObject *object);

/* Image */
static Image *GameObject_get_image(struct GameObject *object);
static int GameObject_get_width(struct GameObject *object);
static int GameObject_get_height(struct GameObject *object);
static char **GameObject_get_pixels(struct GameObject *object);  



/* ----- | Static Variables | ------ */

static GameObject_Methods methods = {
    .free = GameObject_free,
    .is_active = GameObject_is_active,
    .get_owner = GameObject_get_owner,
    .get_controller = GameObject_get_controller,
    .set_controller = GameObject_set_controller,
    .get_direction = GameObject_get_direction,
    .set_direction = GameObject_set_direction,

    .on_tick = GameObject_on_tick,
    .get_order_count = GameObject_get_order_count,
    .get_current_order = GameObject_get_current_order,
    .move_to = GameObject_move_to,
    .attack = GameObject_attack,
    .shoot = GameObject_shoot,

    .movement = GameObject_movement,
    .get_x = GameObject_get_x,
    .get_y = GameObject_get_y,
    .set_x = GameObject_set_x,
    .set_y = GameObject_set_y,
    .set_xy = GameObject_set_xy,
    .get_surface = GameObject_get_surface,

    .get_image = GameObject_get_image,
    .get_width = GameObject_get_width,
    .get_height = GameObject_get_height,
    .get_pixels = GameObject_get_pixels 
};



/* ----- | Functions | ----- */

/*
 * Constructor for GameObjects.
 */
struct GameObject* GameObject_init(struct GameObject *object, Player *owner, int x, int y, const Image *image, 
    Surface_Options *options) {

    (void)options;
    
    if (object == NULL) {
        return NULL;
    }

    object->owner = owner;
    object->x = x;
    object->y = y;
    object->active = TRUE;
    Surface_init_image(&object->surface, image, x, y, NULL);
    memset(&object->order_queue, 0, sizeof(Order) * MAX_ORDERS);
    object->order_queue[0].type = ORDER_TYPE_NONE;
    object->order_count = 0;
    object->current_order_index = 0;
    object->direction = EAST;

    object->m = &methods;
    
    return object;
}

/* Utils */
static void GameObject_free(struct GameObject *object) {
    object->active = FALSE;
    object->owner = NULL;
    object->x = 0;
    object->y = 0;
    Surface *surface = &object->surface;
    surface->free(surface);
    int i;
    for (i = 0; i < MAX_ORDERS; i++) {
        Orders_free(&object->order_queue[i]);
    }
    object->order_queue[0].type = ORDER_TYPE_NONE;
    object->order_count = 0;
    object->current_order_index = 0;
    object->direction = EAST;
}

static boolean GameObject_is_active(struct GameObject *object) {
    return object->active;
}

static Player *GameObject_get_owner(struct GameObject *object) {
    return object->owner;
}

static GameObject_Controller *GameObject_get_controller(struct GameObject *object) {
    return object->controller;
}

static void GameObject_set_controller(struct GameObject *object,
    struct GameObject_Controller *controller) {

    object->controller = controller;
}

static Direction GameObject_get_direction(struct GameObject *object) {
    return object->direction;
}

static void GameObject_set_direction(struct GameObject *object, Direction direction) {
    object->direction = direction;
}



/* Orders */

static void GameObject_on_tick(struct GameObject *object) {
    if (object->order_count == 0) {
        return;
    }
    
    int is_finished = Orders_update(object);
    if (is_finished == TRUE) {
        remove_current_order(object);
    }
}

static int GameObject_get_order_count(struct GameObject *object) {
    return object->order_count;
}

static Order *GameObject_get_current_order(struct GameObject *object) {
    int first_order_index = object->current_order_index;
    return &object->order_queue[first_order_index];
}

static boolean GameObject_move_to(struct GameObject *object, int x, int y, boolean queued) {
    Order order;
    Orders_move(&order, x, y);
    return insert_order(object, &order, queued);
}

static boolean GameObject_attack(struct GameObject *object, struct GameObject *target, boolean queued) {
    Order order;
    Orders_attack(&order, target);
    return insert_order(object, &order, queued);
}

static boolean GameObject_shoot(struct GameObject *object) {
    if (object->controller != NULL) {
        return object->controller->m->shoot(object->controller, object);
    }
    return FALSE;
}



/* Movement and coordinates */
static void GameObject_movement(struct GameObject *object, int x, int y) {
    int width = object->m->get_width(object);
    int height = object->m->get_height(object);
    (void)width; // Stop complaints about unused.
    (void)height; // Stop complaints about unused.
    object->x += x;
    object->y += y;
    Surface *surface = &object->surface;
    surface->movement(surface, x, y);
}

static int GameObject_get_x(struct GameObject *object) {
    return object->x;
}

static int GameObject_get_y(struct GameObject *object) {
    return object->y;
}

static void GameObject_set_x(struct GameObject *object, int x) {
    GameObject_set_xy(object, x, object->y);
}

static void GameObject_set_y(struct GameObject *object, int y) {
    GameObject_set_xy(object, object->x, y);
}

static void GameObject_set_xy(struct GameObject *object, int x, int y) {
    object->x = x;
    object->y = y;
    Surface *surface = &object->surface;
    surface->set_xy(surface, x, y);
}

static Surface *GameObject_get_surface(struct GameObject *object) {
    return &object->surface;
}



/* Image */
static Image *GameObject_get_image(struct GameObject *object) {
    Surface *surface = &object->surface;
    return surface->get_image(surface);
}

static int GameObject_get_width(struct GameObject *object) {
    Surface *surface = &object->surface;
    return surface->get_width(surface);
}

static int GameObject_get_height(struct GameObject *object) {
    Surface *surface = &object->surface;
    return surface->get_height(surface);
}

static char **GameObject_get_pixels(struct GameObject *object) {
    Surface *surface = &object->surface;
    return surface->get_pixels(surface);
}  