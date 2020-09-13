#ifndef ENTITY_CONTAINER_H
#define ENTITY_CONTAINER_H

GameObject;



/* ----- | Type Definitions | ----- */

typedef struct GameObjectContainer {
    unsigned int size;
    GameObject *gameobjects;
    
    GameObjectContainer_methods *m;
} GameObjectContainer;

typedef struct GameObjectContainer_methods {
    void (*free)(struct GameObjectContainer *container);
    void (*on_tick)(struct GameObjectContainer *container);
    int (*get_size)(struct GameObjectContainer *container);
    GameObject *(*get_new_object)(struct GameObjectContainer *container, unsigned int *index);
    GameObject *(*get_object)(struct GameObjectContainer *container, unsigned int index);
    GameObject *(*get_all_objects)(struct GameObjectContainer *container, unsigned int *size);
} GameObjectContainer_methods;

#endif
