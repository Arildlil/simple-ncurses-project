#ifndef SQUARE_H
#define SQUARE_H

#include "utils.h"
#include "terrain_types.h"

struct Map;

typedef struct Square Square;
typedef struct Square_Methods Square_Methods;

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

/*
 * Initialize the given square.
 * .
 * @arg map: The map containing the square.
 * @arg x: The x-coordinate of the square.
 * @arg y: The y-coordinate of the square.
 * @arg type: The terrain type to use.
 * @return: TRUE of success, FALSE otherwise.
 */
boolean Square_init(Square *square, int x, int y, TerrainType_Tag type);

#endif