#include "units.h"
#include "unit_images.h"
#include "curses.h"



/* ----- | Prototypes | ------ */

static GameObject *Units_init(GameObject *object, Player *owner, int x, int y, Image *image);

/* ----- | Static Variables | ------ */

static const Image archer_image;
static const Image swordman_image;
static const Image spearman_image;
static const Image peasant_image;

/* ----- | Functions | ----- */



GameObject *Units_init_archer(GameObject *object, Player *owner, int x, int y) {
    return Units_init(object, owner, x, y, &UNIT_IMAGE_ARCHER);
}

GameObject *Units_init_swordman(GameObject *object, Player *owner, int x, int y) {
    return Units_init(object, owner, x, y, &UNIT_IMAGE_SWORDMAN);
}

GameObject *Units_init_spearman(GameObject *object, Player *owner, int x, int y) {
    return Units_init(object, owner, x, y, &UNIT_IMAGE_SPEARMAN);
}

GameObject *Units_init_peasant(GameObject *object, Player *owner, int x, int y) {
    return Units_init(object, owner, x, y, &UNIT_IMAGE_PEASANT);
}

GameObject *Units_init_name(GameObject *object, Player *owner, int x, int y, char *unit_name) {
    GameObject *(*func_to_call)(GameObject*, Player*, int, int) = NULL;

    if (unit_name == NULL) return NULL;

    if (StrRel(unit_name, ==, "archer")) func_to_call = Units_init_archer;
    if (StrRel(unit_name, ==, "swordman")) func_to_call = Units_init_swordman;
    if (StrRel(unit_name, ==, "spearman")) func_to_call = Units_init_spearman;
    if (StrRel(unit_name, ==, "peasant")) func_to_call = Units_init_peasant;

    if (func_to_call == NULL) {
        return NULL;
    }
    return (*func_to_call)(object, owner, x, y);
}



/*
 * Creates and returns a new unit.
 */
static GameObject *Units_init(GameObject *object, Player *owner, int x, int y, Image *image) {
    return GameObject_init(object, owner, x, y, image, NULL);
}