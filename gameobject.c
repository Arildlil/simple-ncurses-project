#include "gameobject.h"
#include "unit_images.h"
#include "utils.h"



/* ----- | Prototypes | ------ */

/* Utils */
static void GameObject_free(struct GameObject *object);
static boolean GameObject_is_active(struct GameObject *object);
static Player *GameObject_get_owner(struct GameObject *object);

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
struct GameObject* GameObject_init(struct GameObject *object, int x, int y, const Image *image, 
    Surface_Options *options) {
    
    if (object == NULL) {
        return NULL;
    }

    object->x = x;
    object->y = y;
    object->active = TRUE;
    Surface_init_image(&object->surface, image, x, y, NULL);

    object->m = &methods;
    
    return object;
}

/* Utils */
static void GameObject_free(struct GameObject *object) {
    object->active = FALSE;
    object->x = 0;
    object->y = 0;
    Surface *surface = &object->surface;
    surface->free(surface);
}

static boolean GameObject_is_active(struct GameObject *object) {
    return object->active;
}

static Player *GameObject_get_owner(struct GameObject *object) {
    return object->owner;
}

/* Movement and coordinates */
static void GameObject_movement(struct GameObject *object, int x, int y) {
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