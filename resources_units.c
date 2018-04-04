#include "resources_units.h"
#include "resources.h"
#include "unit_images.h"
#include "utils.h"
#include "unit_controllers.h"



/* ----- | Static Variables | ------ */





/* ----- | Functions | ----- */

void for_each_GameObject(void (*func)(GameObject *object)) {
    Resources_for_each(func, RESOURCE_ALL);
}

void free_Unit(GameObject *object) {
    if (object == NULL) {
        return;
    }

    free_GameObject(object);
}

GameObject *new_Unit(Player *owner, int x, int y, char *name) {
    if (owner == NULL || name == NULL) {
        //fprintf(stderr, "new_Unit: Error - 'owner' or 'name' was NULL!\n");
        return NULL;
    }

    Image *image = NULL;
    GameObject_Controller *controller = NULL;

    if (StrRel(name, ==, "archer")) {
        image = &UNIT_IMAGE_ARCHER;
        controller = get_controller_archer();
    }
    if (StrRel(name, ==, "swordman")) {
        image = &UNIT_IMAGE_SWORDMAN;
        controller = get_controller_swordman();
    }
    if (StrRel(name, ==, "spearman")) {
        image = &UNIT_IMAGE_SPEARMAN;
        controller = get_controller_spearman();
    }
    if (StrRel(name, ==, "peasant")) {
        image = &UNIT_IMAGE_PEASANT;
        controller = get_controller_peasant();
    }

    if (image == NULL) {
        return NULL;
    }

    GameObject *object = new_GameObject(RESOURCE_PLAIN);
    if (object == NULL) {
        //fprintf(stderr, "new_Unit: Error - failed to allocate memory for new GameObject!\n");
        return NULL;
    }
    GameObject_init(object, owner, x, y, image, NULL);
    object->m->set_controller(object, controller);
    return object;
}



/* Wrappers */

GameObject *new_Archer(Player *owner, int x, int y) {
    return new_Unit(owner, x, y, "archer");
}

GameObject *new_Swordman(Player *owner, int x, int y) {
    return new_Unit(owner, x, y, "swordman");
}

GameObject *new_Spearman(Player *owner, int x, int y) {
    return new_Unit(owner, x, y, "spearman");
}

GameObject *new_Peasant(Player *owner, int x, int y) {
    return new_Unit(owner, x, y, "peasant");
}