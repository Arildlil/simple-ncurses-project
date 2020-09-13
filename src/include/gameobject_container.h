#ifndef ENTITY_CONTAINER_H
#define ENTITY_CONTAINER_H

#include "gameobject.h"

typedef struct GameObjectContainer GameObjectContainer;
typedef struct GameObjectContainer_methods GameObjectContainer_methods;



/* ----- | Type Definitions | ----- */

struct GameObjectContainer {
    unsigned int size;
    GameObject *gameobjects;
    
    GameObjectContainer_methods *m;
};

struct GameObjectContainer_methods {
    void (*free)(struct GameObjectContainer *container);
    void (*on_tick)(struct GameObjectContainer *container);
    int (*get_size)(struct GameObjectContainer *container);
    GameObject *(*get_new_object)(struct GameObjectContainer *container, unsigned int *index);
    GameObject *(*get_object)(struct GameObjectContainer *container, unsigned int index);
    GameObject *(*get_all_objects)(struct GameObjectContainer *container, unsigned int *size);
};



/* ----- | Functions | ----- */

struct GameObjectContainer *GameObjectContainer_init(struct GameObjectContainer *container,
    unsigned int size);

#endif
