#include "resources.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>



/* ----- | Function Prototypes | ----- */

typedef struct GameObject_Container {
    boolean in_use;
    GameObject object;
} GameObject_Container;



/* ----- | Function Prototypes | ----- */





/* ----- | Static Variables | ------ */

static boolean inited = FALSE;
static GameObject_Container *plain_objects = NULL;
static GameObject_Container *projectiles = NULL;

static size_t plain_objects_max = 0;
static size_t projectiles_max = 0;



/* ----- | Functions | ----- */

boolean Resources_init(size_t max_objects, size_t max_projectiles) {
    if (inited == TRUE) {
        return FALSE;
    }
    assert(max_objects > 0);
    assert(max_projectiles > 0);

    int i;

    plain_objects_max = max_objects;
    plain_objects = calloc(plain_objects_max, sizeof(GameObject_Container));
    if (plain_objects == NULL) {
        fprintf(stderr, "Resources_init: Not enough memory! Exiting...\n");
        return FALSE;
    }
    for (i = 0; i < plain_objects_max; i++) {
        plain_objects[i].in_use = FALSE;
    }

    projectiles_max = max_projectiles;
    projectiles = calloc(projectiles_max, sizeof(GameObject_Container));
    if (projectiles == NULL) {
        fprintf(stderr, "Resources_init: Not enough memory! Exiting...\n");
        return FALSE;
    }
    for (i = 0; i < projectiles_max; i++) {
        projectiles[i].in_use = FALSE;
    }

    inited = TRUE;
    return inited;
}

void Resources_exit() {
    if (inited == FALSE) {
        return;
    }

    if (plain_objects != NULL) {
        free(plain_objects);
        plain_objects = NULL;
        plain_objects_max = 0;
    }
    if (projectiles != NULL) {
        free(projectiles);
        projectiles = NULL;
        projectiles_max = 0;
    }

    inited = FALSE;
}

static void on_tick_container(GameObject_Container *container, size_t container_size) {
    assert(container);

    size_t i;
    for (i = 0; i < container_size; i++) {
        GameObject_Container *current = &container[i];
        
        if (current->in_use == TRUE) {
            GameObject *object = &current->object;
            GameObject_Controller *controller = object->m->get_controller(object);
            if (controller != NULL) {
                controller->m->on_tick(controller, object);
            }
        }
    }
}

void Resources_on_tick() {
    on_tick_container(plain_objects, plain_objects_max);
    on_tick_container(projectiles, projectiles_max);
}

GameObject *new_GameObject(enum Resources_Type type) {
    if (inited == FALSE) {
        return NULL;
    }

    GameObject_Container *container;
    int container_size;
    switch (type) {
        case RESOURCE_PLAIN:
            container = plain_objects;
            container_size = plain_objects_max;
            break;
        case RESOURCE_PROJECTILE:
            container = projectiles;
            container_size = projectiles_max;
            break;
        default:
            fprintf(stderr, "new_GameObject: Error - invalid Resource_Type!\n");
            return NULL;
    }

    /* Currently using the First-Fit algorithm */
    int i;
    for (i = 0; i < container_size; i++) {
        GameObject_Container *current = &container[i];
        if (current->in_use == FALSE) {
            current->in_use = TRUE;
            return &current->object;
        }
    }
    return NULL;
}

static boolean free_in_container(GameObject_Container *container, size_t size, GameObject *object) {
    assert(container);
    
    size_t i;
    for (i = 0; i < size; i++) {
        if (container[i].in_use && &container[i].object == object) {
            container[i].in_use = FALSE;
            return TRUE;
        }
    }
    return FALSE;
}

void free_GameObject(GameObject *object) {
    if (free_in_container(plain_objects, plain_objects_max, object) == TRUE) {return;}
    if (free_in_container(projectiles, projectiles_max, object) == TRUE) {return;}
}