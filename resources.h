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
    RESOURCE_PLAIN, RESOURCE_PROJECTILE
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
 * Call the on_tick methods of all the objects in use.
 */
void Resources_on_tick();

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