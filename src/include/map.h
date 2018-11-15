#ifndef TERRAIN_H
#define TERRAIN_H

#include "utils.h"
#include "image.h"
#include "colors.h"
#include "terrain_types.h"
#include "square.h"

typedef struct Map Map;
typedef struct Map_Methods Map_Methods;

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
 * Initialize the Square object at the given coordinates.
 * 
 * @arg map: The Map object where the square is located.
 * @arg x: X coordinate
 * @arg y: Y coordinate
 * @return: TRUE on success, FALSE otherwise.
 */
boolean Map_set_square(Map *map, int x, int y, TerrainType_Tag type);

#endif