#include "resources_units.h"
#include "resources.h"
#include "unit_images.h"
#include "utils.h"



GameObject *new_Unit(Player *owner, int x, int y, char *name) {
    if (owner == NULL || name == NULL) {
        //fprintf(stderr, "new_Unit: Error - 'owner' or 'name' was NULL!\n");
        return NULL;
    }

    Image *image = NULL;
    if (StrRel(name, ==, "archer")) image = &UNIT_IMAGE_ARCHER;
    if (StrRel(name, ==, "swordman")) image = &UNIT_IMAGE_SWORDMAN;
    if (StrRel(name, ==, "spearman")) image = &UNIT_IMAGE_SPEARMAN;
    if (StrRel(name, ==, "peasant")) image = &UNIT_IMAGE_PEASANT;

    if (image == NULL) {
        return NULL;
    }

    GameObject *object = new_GameObject(RESOURCE_PLAIN);
    if (object == NULL) {
        //fprintf(stderr, "new_Unit: Error - failed to allocate memory for new GameObject!\n");
        return NULL;
    }
    return GameObject_init(object, owner, x, y, image, NULL);
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