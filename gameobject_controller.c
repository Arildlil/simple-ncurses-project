#include "gameobject.h"



/* ----- | Prototypes | ------ */

static void GameObject_Controller_Methods_on_tick(struct GameObject_Controller *controller, GameObject *object);



/* ----- | Static Variables | ------ */

static GameObject_Controller_Methods default_methods = {
    .on_tick = GameObject_Controller_Methods_on_tick
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
    object->m->movement(object, 1, 0);
}