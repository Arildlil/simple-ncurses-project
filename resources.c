#include "resources.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>



/* ----- | Constants | ----- */

#define ERROR_NO_MEMORY "Error: not enough memory! Exiting...\n"
#define ERROR_FULL "Error: no space for new GameObject!\n"



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

GameObject **used_objects = NULL;
static GameObject_Container *items = NULL;

static size_t max_size = 0;
static size_t cur_size = 0;
static size_t first_free = 0;



/* ----- | Functions | ----- */

boolean Resources_init(size_t max_objects) {
    if (inited == TRUE) {
        return FALSE;
    }
    assert(max_objects > 0);

    max_size = max_objects;
    cur_size = 0;
    first_free = 0;

    items = calloc(max_size, sizeof(GameObject_Container));
    if (items == NULL) {
        //fprintf(stderr, ERROR_NO_MEMORY);
        return FALSE;
    }

    used_objects = calloc(max_size, sizeof(GameObject*));
    if (used_objects == NULL) {
        //fprintf(stderr, ERROR_NO_MEMORY);
        return FALSE;
    }

    size_t i;
    for (i = 0; i < max_size; i++) {
        items[i].in_use = FALSE;
        used_objects[i] = NULL;
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
    first_free = 0;

    if (used_objects != NULL) {
        free(used_objects);
        used_objects = NULL;
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

void Resources_on_tick() {
    size_t i, counted;
    for (i = 0, counted = 0; i < max_size && counted < cur_size; i++, counted++) {
        if (used_objects[i] != NULL) {
            GameObject *object = used_objects[i];
            GameObject_Controller *controller = object->m->get_controller(object);
            if (controller != NULL && controller->m->on_tick != NULL) {
                controller->m->on_tick(controller, object);
            }
        }
    }
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
    (void)type;

    if (inited == FALSE) {
        return NULL;
    }

    if (cur_size >= max_size || first_free >= max_size) {
        //fprintf(stderr, ERROR_FULL);
        return NULL;
    }

    GameObject_Container *current_container = &items[first_free];
    assert(current_container != NULL);
    assert(current_container->in_use == FALSE);
    current_container->in_use = TRUE; 
    GameObject *object = &current_container->object;
    object->memory_id = first_free;
    cur_size++;

    //fprintf(stderr, "first_free was %d!\n", first_free);

    used_objects[object->memory_id] = object;

    size_t i;
    for (i = first_free; i < max_size; i++, first_free++) {
        if (items[i].in_use == FALSE) {
            break;
        }
    }

    //fprintf(stderr, "first_free is now %d!\n", first_free);

    return object;
}

void free_GameObject(GameObject *object) {
    if (object == NULL) {
        return;
    }

    size_t memory_id = object->memory_id;
    assert(memory_id < max_size);
    items[memory_id].in_use = FALSE;
    used_objects[memory_id] = NULL;

    cur_size--;
    if (memory_id < first_free) {
        first_free = memory_id;
    }
}