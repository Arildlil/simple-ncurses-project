#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "curses.h"
#include "utils.h"
#include "gameobject.h"
#include "player.h"

/* Necessary forward declaration */
typedef struct GameObject GameObject;
typedef struct GameObject_Methods GameObject_Methods;
typedef struct GameObject_Controller GameObject_Controller;
typedef struct GameObject_Controller_Methods GameObject_Controller_Methods;

struct GameObject {
    Player *owner;
    int x;
    int y;
    Surface surface;
    boolean active;

    /* Controls the behavior of this object, if set */
    GameObject_Controller *controller;

    /* Common methods for all GameObjects */
    GameObject_Methods *m;
};

struct GameObject_Controller {
    /* Contains function pointers. These can vary from 
     * object to object. */
    GameObject_Controller_Methods *m;
};

struct GameObject_Methods {
    /* Utils */
    void (*free)(struct GameObject *object);
    boolean (*is_active)(struct GameObject *object);
    Player *(*get_owner)(struct GameObject *object);

    /* Movement and coordinates */
    void (*movement)(struct GameObject *object, int x, int y);
    int (*get_x)(struct GameObject *object);
    int (*get_y)(struct GameObject *object);
    void (*set_x)(struct GameObject *object, int x);
    void (*set_y)(struct GameObject *object, int y);
    void (*set_xy)(struct GameObject *object, int x, int y);
    Surface *(*get_surface)(struct GameObject *object);

    /* Image */
    Image *(*get_image)(struct GameObject *object);
    int (*get_width)(struct GameObject *object);
    int (*get_height)(struct GameObject *object);
    char **(*get_pixels)(struct GameObject *object);
};

/* This struct should be filled manually! */
struct GameObject_Controller_Methods {
    void (*on_tick)(struct GameObject_Controller *controller, struct GameObject *object);
};

/*
 * Constructor for GameObjects.
 * 
 * @arg object: The object to initialize.
 * @arg x: The x-coordinate to spawn at.
 * @arg y: The y-coordinate to spawn at.
 * @arg image: The image to use. If this is NULL, the GameObject will NOT be rendered!
 * @arg options (Optional): The surface options to use. Uses default options if NULL.
 * @return: The object argument.
 */
GameObject* GameObject_init(GameObject *object, Player *owner, int x, int y, const Image *image,
    Surface_Options *options);

/*
 * Constructor for GameObject_Controllers. One controller can be shared by several GameObjects.
 * 
 * @arg controller: The controller object to initialize.
 * @arg methods: A pointer to a struct with function pointers, working as methods for the object.
 * @return: The controller argument.
 */
GameObject_Controller* GameObject_Controller_init(GameObject_Controller *controller, 
    struct GameObject_Controller_Methods *methods);

#endif