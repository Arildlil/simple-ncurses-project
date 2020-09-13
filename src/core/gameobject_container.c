#include "../../include/gameobject_container.h"

#include <stdlib.h>



/* ----- | Prototypes | ------ */

static void GameObjectContainer_free(struct GameObjectContainer *container);
static void GameObjectContainer_on_tick(struct GameObjectContainer *container);
static int GameObjectContainer_get_size(struct GameObjectContainer *container);
static GameObject *GameObjectContainer_get_new_object(struct GameObjectContainer *container, unsigned int *index);
static GameObject *GameObjectContainer_get_object(struct GameObjectContainer *container, unsigned int index);
static GameObject *GameObjectContainer_get_all_objects(struct GameObjectContainer *container, unsigned int *size);



/* ----- | Static Variables | ------ */

static GameObjectContainer_methods methods = {
    .free = GameObjectContainer_free,
    .on_tick = GameObjectContainer_on_tick,
    .get_size = GameObjectContainer_get_size,
    .get_new_object = GameObjectContainer_get_new_object,
    .get_object = GameObjectContainer_get_object,
    .get_all_objects = GameObjectContainer_get_all_objects
}



/* ----- | Functions | ----- */

struct GameObjectContainer GameObjectContainer_init(struct GameObjectContainer *container,
    unsigned int size) {
   
    if (container == NULL) {
        return NULL;  
    }
    
    container->gameobjects = calloc(size, sizeof(GameObject));
    container->size = container->gameobjects == NULL ? 0 : size;
    
    container->m = &methods;
    
    return container;
}

static void GameObjectContainer_free(struct GameObjectContainer *container) {
    if (container == NULL) {
        return;
    }
    
    if (container->size > 0) {
        free(container->gameobjects);
        container->size = 0;
    }
}

static void GameObjectContainer_on_tick(struct GameObjectContainer *container) {
    if (container == NULL) {
        return;
    }
    
    int index;
    for (index = 0; index < container->size; index++) {
        GameObject *current = GameObjectContainer_get_object(container, index);
        
        if (current != NULL) {
            current->m->on_tick(current);
        }
    }
}

static int GameObjectContainer_get_size(struct GameObjectContainer *container) {
    if (container == NULL) {
        return -1;
    }
    
    return container->size;
}

static GameObject *GameObjectContainer_get_new_object(struct GameObjectContainer *container, unsigned int *index) {
    if (container == NULL) {
        return NULL;
    }
    
    int current_index;
    for (current_index = 0; current_index < container->size; current_index++) {
        GameObject *current = container[current_index];
        if (!current->m->is_active(current)) {
            *index = current_index;
            return GameObjectContainer_get_object(container, current_index);
        }
    }
    
    return NULL;
}

static GameObject *GameObjectContainer_get_object(struct GameObjectContainer *container, unsigned int index) {
    if (container == NULL) {
        return NULL;
    }
    
    // May choose to return an object in the future, by using the NullObjectPattern instead of NULL.
    if (index >= size) {
        return NULL;
    }
    
    return container[index];
}

static GameObject *GameObjectContainer_get_all_objects(struct GameObjectContainer *container, unsigned int *size) {
    if (container == NULL) {
        return;
    }
    
    *size = container->size;
    return container->gameobjects;
}
