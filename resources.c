#include "resources.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>



/* ----- | Constants | ----- */

static const char *ERROR_NO_MEMORY = "Error: not enough memory! Exiting...\n";



/* ----- | Function Prototypes | ----- */

typedef struct GameObject_Container {
    boolean in_use;
    GameObject object;
} GameObject_Container;



/* ----- | Function Prototypes | ----- */

/* TODO:
    * Used-array
    * Free-array
    * Update upon allocation or deallocation
*/



/* ----- | Static Variables | ------ */

static boolean inited = FALSE;

static GameObject_Container *used_objects = NULL;
static GameObject_Container *free_objects = NULL;
static GameObject_Container *items = NULL;

static size_t max_size = 0;
static size_t cur_size = 0;



/* ----- | Functions | ----- */

boolean Resources_init(size_t max_objects) {
    if (inited == TRUE) {
        return FALSE;
    }
    assert(max_objects > 0);

    size_t i;

    max_size = max_objects;
    cur_size = 0;

    used_objects = calloc(max_size, sizeof(GameObject_Container*));
    free_objects = calloc(max_size, sizeof(GameObject_Container*));
    if (used_objects == NULL || free_objects == NULL) {
        fprintf(stderr, ERROR_NO_MEMORY);
        return FALSE;
    }

    items = calloc(max_size, sizeof(GameObject_Container));
    if (items == NULL) {
        fprintf(stderr, ERROR_NO_MEMORY);
        return FALSE;
    }
    for (i = 0; i < max_size; i++) {
        items[i].in_use = FALSE;
    }

    inited = TRUE;
    return inited;
}

void Resources_exit() {
    if (inited == FALSE) {
        return;
    }

    max_size = 0;
    cur_size = 0;

    if (used_objects != NULL) {
        free(used_objects);
        used_objects = NULL;
    }

    if (free_objects != NULL) {
        free(free_objects);
        free_objects = NULL;
    }

    if (items != NULL) {
        free(items);
        items = NULL;
    }

    inited = FALSE;
}

size_t Resources_max_objects() {
    return max_size;
}

static void on_tick_container(GameObject_Container *container, size_t container_size) {
    assert(container);

    size_t i;
    for (i = 0; i < container_size; i++) {
        GameObject_Container *current = &container[i];
        
        if (current->in_use == TRUE) {
            GameObject *object = &current->object;
            //object->m->on_tick(object);
            GameObject_Controller *controller = object->m->get_controller(object);
            if (controller != NULL && controller->m->on_tick != NULL) {
                controller->m->on_tick(controller, object);
            }
        }
    }
}

void Resources_on_tick() {
    on_tick_container(items, max_size);
}

static void for_each_container(GameObject_Container *container, size_t container_size, 
    void (*func)(GameObject *object)) {

    size_t i;
    for (i = 0; i < container_size; i++) {
        GameObject_Container *current = &container[i];
        if (current->in_use == TRUE) {
            func(&current->object);
        }
    }
}

void Resources_for_each(void (*func)(GameObject *object), enum Resources_Type type) {
    switch (type) {
        case RESOURCE_ALL:
            /* Intentional fall-through */
        case RESOURCE_PLAIN:
            /* Intentional fall-through */
        case RESOURCE_PROJECTILE:
            for_each_container(items, max_size, func);
            break;
    }
}

GameObject *new_GameObject(enum Resources_Type type) {
    if (inited == FALSE) {
        return NULL;
    }

    GameObject_Container *container;
    int container_size;
    switch (type) {
        case RESOURCE_PLAIN:
            /* Intentional fall-through */
        case RESOURCE_PROJECTILE:
            container = items;
            container_size = max_size;
            break;
        default:
            fprintf(stderr, "new_GameObject: Error - invalid Resource_Type!\n");
            return NULL;
    }

    /* TODO: Stop using resource types and rather have one array instead? */

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
    if (free_in_container(items, max_size, object) == TRUE) {return;}
}