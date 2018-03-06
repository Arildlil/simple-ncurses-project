#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "curses.h"

typedef struct GameObject {
    int x;
    int y;
    Surface surface;
    boolean active;

    /* Utils */
    void (*free)(struct GameObject *object);
    boolean (*is_active)(struct GameObject *object);

    /* Movement and coordinates */
    void (*movement)(struct GameObject *object, int x, int y);
    int (*get_x)(struct GameObject *object);
    int (*get_y)(struct GameObject *object);
    void (*set_x)(struct GameObject *object, int x);
    void (*set_y)(struct GameObject *object, int y);
    void (*set_xy)(struct GameObject *object, int x, int y);

    /* Image */
    Image *(*get_image)(struct GameObject *object);
    int (*get_width)(struct GameObject *object);
    int (*get_height)(struct GameObject *object);
    char **(*get_pixels)(struct GameObject *object);
} GameObject;

/*
 * Constructor for GameObjects.
 * 
 * @arg object: The object to initialize.
 * @arg x: The x-coordinate to spawn at.
 * @arg y: The y-coordinate to spawn at.
 * @arg image: The image to use.
 * @return: The object argument.
 */
GameObject* GameObject_init(GameObject *object, int x, int y, const Image *image);

#endif