#ifndef TERRAIN_H
#define TERRAIN_H

#include "utils.h"

typedef struct Square Square;
typedef struct Square_Methods Square_Methods;
typedef enum Terrain_Type Terrain_Type;
typedef struct Map Map;
typedef struct Map_Methods Map_Methods;

enum Terrain_Type {
    TERRAIN_GRASS, TERRAIN_TREE, TERRAIN_WATER, TERRAIN_STONE
};

struct Square {
    Terrain_Type terrain;
    int x;
    int y;

    Square_Methods *m;
};

struct Square_Methods {
    Terrain_Type (*get_terrain_type)(struct Square *square);
    int (*get_x)(struct Square *square);
    int (*get_y)(struct Square *square);
};

struct Map {
    boolean inited;
    int max_x;
    int min_x;
    int max_y;
    int min_y;
    int width;
    int height;
    Square **squares;

    Map_Methods *m;
};

struct Map_Methods {
    boolean (*is_inited)(struct Map *map);
    int (*get_max_x)(struct Map *map);
    int (*get_max_y)(struct Map *map);
    int (*get_width)(struct Map *map);
    int (*get_height)(struct Map *map);
    Square *(*get_square)(struct Map *map, int x, int y);
    void (*free)(struct Map *map);
};

/*
 * Initialize the Map object.
 * 
 * @arg map: The Map object to initialize.
 * @arg max_x: Max positive and negative x (-max_x to max_x)
 * @arg max_y: Max positive and negative y (-max_y to max_y)
 * @return: TRUE on success, FALSE otherwise.
 */
boolean Map_init(Map *map, int max_x, int max_y);

/*
 * Initialize the given square.
 * .
 * @arg map: The map containing the square.
 * @arg x: The x-coordinate of the square.
 * @arg y: The y-coordinate of the square.
 * @arg type: The terrain type to use.
 * @return: TRUE of success, FALSE otherwise.
 */
boolean Square_init(Map *map, int x, int y, enum Terrain_Type type);

#endif