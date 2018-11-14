#ifndef TERRAIN_H
#define TERRAIN_H

#include "include/utils.h"
#include "include/image.h"
#include "include/colors.h"
#include "include/terrain_types.h"

typedef struct Square Square;
typedef struct Square_Methods Square_Methods;
typedef struct Map Map;
typedef struct Map_Methods Map_Methods;

struct Square {
    TerrainType *terrain;
    int x;
    int y;

    Square_Methods *m;
};

struct Square_Methods {
    TerrainType *(*get_terrain_type)(struct Square *square);
    int (*get_x)(struct Square *square);
    int (*get_y)(struct Square *square);
    void (*set_terrain)(struct Square *square, TerrainType_Tag tag);
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
    int (*get_min_x)(struct Map *map);
    int (*get_min_y)(struct Map *map);
    int (*get_width)(struct Map *map);
    int (*get_height)(struct Map *map);
    Square *(*get_square)(struct Map *map, int x, int y);
    void (*free)(struct Map *map);
};

extern Map *global_map;

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
boolean Square_init(Map *map, int x, int y, TerrainType_Tag type);

#endif