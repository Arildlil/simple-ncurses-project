#include "terrain.h"

#include <stdlib.h>
#include <assert.h>



/* ----- | Prototypes | ------ */

static boolean Map_is_inited(struct Map *map);
static int Map_get_max_x(struct Map *map);
static int Map_get_max_y(struct Map *map);
static int Map_get_min_x(struct Map *map);
static int Map_get_min_y(struct Map *map);
static int Map_get_width(struct Map *map);
static int Map_get_height(struct Map *map);
static Square *Map_get_square(struct Map *map, int x, int y);
static void Map_free(struct Map *map);

static TerrainType *Square_get_terrain_type(struct Square *square);
static int Square_get_x(struct Square *square);
static int Square_get_y(struct Square *square);
static void Square_set_terrain(struct Square *square, TerrainType_Tag tag);



/* ----- | Static Variables | ------ */

static Map_Methods map_methods = {
    .is_inited = Map_is_inited,
    .get_max_x = Map_get_max_x,
    .get_max_y = Map_get_max_y,
    .get_min_x = Map_get_min_x,
    .get_min_y = Map_get_min_y,
    .get_width = Map_get_width,
    .get_height = Map_get_height,
    .get_square = Map_get_square,
    .free = Map_free
};

static Square_Methods square_methods = {
    .get_terrain_type = Square_get_terrain_type,
    .get_x = Square_get_x,
    .get_y = Square_get_y,
    .set_terrain = Square_set_terrain,
};

Map *global_map = NULL;



/* ----- | Functions | ----- */

/*
 * Constructor for Map.
 */
boolean Map_init(Map *map, int max_x, int max_y) {
    if (max_x < 0 || max_y < 0) {
        return FALSE;
    }

    int i;
    for (i = 0; i < TERRAIN_ENUM_SIZE; i++) {
        TerrainType *current_type = &terrain_types[i];
        Image_init_2D(current_type->image, current_type->width, current_type->height, 
            TERRAIN_STRINGS[current_type->tag]);
        
    }

    map->inited = TRUE;
    map->max_x = max_x;
    map->min_x = max_x * (-1);
    map->max_y = max_y;
    map->min_y = max_y * (-1);
    map->width = max_x * 2 + 1;
    map->height = max_y * 2 + 1;
    map->m = &map_methods;

    int width = map->width;
    int height = map->height;
    /*fprintf(stderr, "Map_init(map, max_x = %d, max_y = %d)\n", max_x, max_y);
    fprintf(stderr, "width = %d, height = %d\n", width, height);*/
    map->squares = calloc(height, sizeof(Square*));

    int h, w;
    for (h = 0; h < height; h++) {
        map->squares[h] = calloc(width, sizeof(Square));
        for (w = 0; w < width; w++) {
            Square_init(map, w - max_x, h - max_y, TERRAIN_NONE);
        }
    }

    global_map = map;

    return TRUE;   
}

/*
 * Constructor for Square.
 */
boolean Square_init(Map *map, int x, int y, TerrainType_Tag type) {
    if (map == NULL) {
        return FALSE;
    }
    if (map->inited != TRUE) {
        return FALSE;
    }
    
    Square *square = map->m->get_square(map, x, y);
    if (square == NULL) {
        /*fprintf(stderr, "Square_init: Error - 'square' is NULL (x = %d, y = %d)!\n", x, y);*/
        return FALSE;
    }

    square->m = &square_methods;
    square->terrain = &terrain_types[type];
    square->x = x;
    square->y = y;

    return TRUE;
}

/* Map */
static boolean Map_is_inited(struct Map *map) {
    return map->inited;
}

static int Map_get_max_x(struct Map *map) {
    return map->max_x;
}

static int Map_get_max_y(struct Map *map) {
    return map->max_y;
}

static int Map_get_min_x(struct Map *map) {
    return map->min_x;
}

static int Map_get_min_y(struct Map *map) {
    return map->min_y;
}

static int Map_get_width(struct Map *map) {
    return map->width;
}

static int Map_get_height(struct Map *map) {
    return map->height;
}

static Square *Map_get_square(struct Map *map, int x, int y) {
    if (x > map->max_x || x < map->min_x || y > map->max_y || y < map->min_y) {
        /*fprintf(stderr, "Map_get_square: Error - 'x' or 'y' outside range of (x: %d - %d, y: %d - %d)! Was (x: %d, y: %d)\n",
            map->min_x, map->max_x, map->min_y, map->max_y, x, y);*/
        return NULL;
    }
    int i = x + map->max_x;
    int j = y + map->max_y;
    /*fprintf(stderr, "Map_get_square: (x = %d, y = %d), (i = %d, j = %d)\n", x, y, i, j);*/
    Square *row = map->squares[j];
    return &row[i];
}

static void Map_free(struct Map *map) {
    int i;
    for (i = 0; i < map->height; i++) {
        if (map->squares[i] != NULL) {
            free(map->squares[i]);
        }
        map->squares[i] = NULL;
    }
    free(map->squares);
    map->squares = NULL;

    map->inited = FALSE;
    map->max_x = 0;
    map->max_y = 0;
    map->width = 0;
    map->height = 0;
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
