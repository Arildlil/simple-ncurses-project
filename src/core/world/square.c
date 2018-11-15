#include "../../include/image.h"
#include "../../include/square.h"


/* ----- | Prototypes | ------ */

static TerrainType *Square_get_terrain_type(struct Square *square);
static int Square_get_x(struct Square *square);
static int Square_get_y(struct Square *square);
static void Square_set_terrain(struct Square *square, TerrainType_Tag tag);



/* ----- | Static Variables | ------ */

static Square_Methods square_methods = {
    .get_terrain_type = Square_get_terrain_type,
    .get_x = Square_get_x,
    .get_y = Square_get_y,
    .set_terrain = Square_set_terrain,
};



/* ----- | Functions | ----- */

/*
 * Constructor for Square.
 */
boolean Square_init(Square *square, int x, int y, TerrainType_Tag type) {
    if (square == NULL) {
        return FALSE;
    }
    
    /*
    Square *square = map->m->get_square(map, x, y);

    if (square == NULL) {
        //fprintf(stderr, "Square_init: Error - 'square' is NULL (x = %d, y = %d)!\n", x, y);
        return FALSE;
    }
    */

    square->m = &square_methods;
    square->terrain = &terrain_types[type];
    square->x = x;
    square->y = y;

    return TRUE;
}

/* Square */
static TerrainType *Square_get_terrain_type(struct Square *square) {
    return square->terrain;
}

static int Square_get_x(struct Square *square) {
    return square->x;
}

static int Square_get_y(struct Square *square) {
    return square->y;
}

static void Square_set_terrain(struct Square *square, TerrainType_Tag tag) {
    //assert(tag < TERRAIN_ENUM_SIZE && tag >= 0);
    square->terrain = &terrain_types[tag];
}