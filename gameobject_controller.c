#include "gameobject.h"



/* ----- | Prototypes | ------ */

static void GameObject_Controller_Methods_on_tick(struct GameObject_Controller *controller, GameObject *object);
static boolean GameObject_Controller_Methods_shoot(struct GameObject_Controller *controller, GameObject *object);



/* ----- | Static Variables | ------ */

static GameObject_Controller_Methods default_methods = {
    .on_tick = GameObject_Controller_Methods_on_tick,
    .shoot = GameObject_Controller_Methods_shoot,
};



/* ----- | Functions | ----- */

GameObject_Controller* GameObject_Controller_init(GameObject_Controller *controller, 
    struct GameObject_Controller_Methods *methods) {

    if (controller == NULL) {
        return NULL;
    }

    /* Use default methods if not provided */
    if (methods == NULL) {
        controller->m = &default_methods;
        return controller;
    }
    
    controller->m = methods;

    return controller;
}

static void GameObject_Controller_Methods_on_tick(GameObject_Controller *controller, GameObject *object) {
    (void)controller;
    object->m->movement(object, 1, 0);
}

static void projectile_on_tick(GameObject *object) {
    int old_x = object->m->get_x(object);
    int old_y = object->m->get_y(object);
    
    int movement_x = 0;
    int movement_y = 0;
    switch (object->m->get_direction(object)) {
        case NORTH_WEST:
            movement_x--;
            movement_y--;
            break;
        case WEST:
            movement_x--;
            break;
        case SOUTH_WEST:
            movement_x--;
            movement_y++;
            break;
        case SOUTH:
            movement_y++;
            break;
        case SOUTH_EAST:
            movement_x++;
            movement_y++;
            break;
        case EAST:
            movement_x++;
            break;
        case NORTH_EAST:
            movement_x++;
            movement_y--;
            break;
        case NORTH:
            movement_y--;
            break;
    }

    int new_x = old_x + movement_x;
    int new_y = old_y + movement_y;
    object->m->move_to(object, new_x, new_y, FALSE);
}

static GameObject *create_projectile(Player *owner, Direction direction, int x, int y) {
    return NULL;
    /*
    const char *projectile_string = "*";
    Image image;
    Image_init_1D(&image, 1, projectile_string);
    GameObject_Controller_Methods methods = {.on_tick = projectile_on_tick};
    GameObject_Controller projectile_controller;
    GameObject_Controller_init(&projectile_controller, &methods);

    GameObject *projectile = malloc(sizeof(GameObject));
    if (projectile == NULL) {return NULL;}

    GameObject_init(projectile, owner, x, y, &image, NULL);
    projectile->m->set_controller(projectile, &projectile_controller);
    projectile->m->set_direction(projectile, direction);
    return projectile;
    */
} 

static boolean GameObject_Controller_Methods_shoot(struct GameObject_Controller *controller, GameObject *object) {
    (void)controller;
    /*
    GameObject *projectile = create_projectile(object->m->get_owner(object), 
        object->m->get_direction(object), object->m->get_x(object), object->m->get_y(object));
    if (projectile == NULL) {
        return FALSE;
    }
    */
    return TRUE;
}