#include "../include/unit_defs.h"
#include "../include/utils.h"
#include "../include/image.h"
#include "../include/surface.h"
#include "../include/unit_controllers.h"

#include <stdio.h>
#include <stdlib.h>



/* ----- | Type Definitions | ----- */

typedef struct UnitDef {
    int width;
    int height;
    const char *name;
    const char *controller;
    const char *pixels[UNIT_IMAGE_WIDTH * UNIT_IMAGE_HEIGHT];
} UnitDef;



/* ----- | Static Globals | ----- */

#define NUM_UNIT_TYPES 4
static boolean inited = FALSE;
static Image images[NUM_UNIT_TYPES];

static UnitDef defs[NUM_UNIT_TYPES] = {
    {
        .width = UNIT_IMAGE_WIDTH,
        .height = UNIT_IMAGE_HEIGHT,
        .name = "archer",
        .controller = "default",
        .pixels = {
            "|\\ ",
            "+->", 
            "|/ ",
        },
    },
    {
        .width = UNIT_IMAGE_WIDTH,
        .height = UNIT_IMAGE_HEIGHT,
        .name = "swordman",
        .controller = "default",
        .pixels = {
            " | ", 
            " | ",
            "-+-",
        },
    },
    {
        .width = UNIT_IMAGE_WIDTH,
        .height = UNIT_IMAGE_HEIGHT,
        .name = "spearman",
        .controller = "default",
        .pixels = {
            " ^ ",
            " | ",
            " | ",
        },
    },
    {
        .width = UNIT_IMAGE_WIDTH,
        .height = UNIT_IMAGE_HEIGHT,
        .name = "peasant",
        .controller = "default",
        .pixels = {
            "\\|/",
            " | ",
            " | ",
        },
    },
};



/* ----- | Function Prototypes | ----- */





/* ----- | Functions | ----- */

void UnitDefs_init() {
    if (inited != FALSE) {
        return;
    }

    inited = TRUE;

    int i;
    for (i = 0; i < NUM_UNIT_TYPES; i++) {
        Image_init_2D(&images[i], defs[i].width, defs[i].height, defs[i].pixels);
    }
}

GameObject *Unit_new(Player *owner, const char *name, int x, int y) {
    if (owner == NULL || name == NULL) {
        return NULL;
    }
    if (inited == FALSE) {
        UnitDefs_init();
    }

    GameObject *object = malloc(sizeof(GameObject));
    if (object == NULL) {
        fprintf(stderr, "(ERROR) Unit_new: Couldn't alloc memory for new unit!\n");
        return NULL;
    }

    Image *image_to_use = NULL;
    GameObject_Controller *controller = NULL;
    int i;
    for (i = 0; i < NUM_UNIT_TYPES; i++) {
        if (StrRel(name, ==, defs[i].name)) {
            image_to_use = &images[i];
            controller = get_controller(defs[i].controller);
        }
    }
    if (image_to_use == NULL) {
        fprintf(stderr, "(ERROR) Unit_new: Couldn't find definition for name \'%s\'!\n", 
            name);
        free(object);
        return NULL;
    } 
    if (controller == NULL) {
        fprintf(stderr, "(WARNING) Unit_new: Couldn't get controller for name \'%s\'! Using default...\n",
            name);
        controller = get_controller("default");
    }
    GameObject_init(object, owner, x, y, image_to_use, NULL);
    object->m->set_controller(object, controller);

    return object;
}