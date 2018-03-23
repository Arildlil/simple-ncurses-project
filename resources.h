#ifndef RESOURCES_H
#define RESOURCES_H

#include "gameobject.h"



/* ----- | Typedefs | ----- */



/* ----- | Constants | ----- */

#define DEFAULT_MAX_GAMEOBJECT_COUNT 100

/* Projectiles are also GameObjects, but they have 
 * their own memory pool */
#define DEFAULT_MAX_PROJECTILE_COUNT 100



/* ----- | Type Definitions | ----- */

typedef enum Resources_Type {
    RESOURCE_ALL, RESOURCE_PLAIN, RESOURCE_PROJECTILE
} Resources_Type;



/* ----- | Constructors & Functions | ----- */

/*
 * Initialize the module, by setting aside the necessary memory
 * and such.
 * 
 * @arg max_objects: The maximum number of normal GameObjects to store.
 * @arg max_projectiles: The maximum number of projectile GameObjects to store.
 * @return: TRUE on success, FALSE otherwise.
 */
boolean Resources_init(size_t max_objects, size_t max_projectiles);

/*
 * Free up all the used memory of the Resource handler. Should
 * only be called when the game exits.
 */
void Resources_exit();

/*
 * Returns the maximum number of GameObjects that can be stored with
 * the currently allocated memory pool.
 * 
 * @return: The maximum number of objects that can be stored.
 */
size_t Resources_max_objects();

/*
 * Call the on_tick methods of all the objects in use.
 */
void Resources_on_tick();

/* 
 * Map a function over all the currently active GameObjects.
 * 
 * @arg func: The mapping function.
 * @arg type: The type of GameObject to map over.
 */
void Resources_for_each(void (*func)(GameObject *object), enum Resources_Type type);

/*
 * Get a fresh GameObject.
 * 
 * @arg type: The type of GameObject to get. 
 * @return: A new GameObject on success, NULL otherwise.
 */
GameObject *new_GameObject(enum Resources_Type type);

/*
 * Free a GameObject that is no longer needed. Will be 
 * put back into the object pool.
 * 
 * @arg object: The GameObject to free.
 */
void free_GameObject(GameObject *object);

#endif