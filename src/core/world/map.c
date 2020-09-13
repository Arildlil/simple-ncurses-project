#include "../../include/map.h"

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
static Square ***Map_get_surrounding_squares(struct Map *map, Square *square);
static void Map_free(struct Map *map);



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
    .get_surrounding_squares = Map_get_surrounding_squares,
    .free = Map_free
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
            Map_set_square(map, w - max_x, h - max_y, TERRAIN_NONE);
        }
    }

    global_map = map;

    return TRUE;   
}

boolean Map_set_square(Map *map, int x, int y, TerrainType_Tag type) {
    if (map == NULL) {
        return FALSE;
    }
    Square *square = map->m->get_square(map, x, y);
    return Square_init(square, x, y, type);
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

static Square ***Map_get_surrounding_squares(struct Map *map, Square *square) {
    static Square *surrounding_squares[3][3] = { NULL };

    int square_x = square->m->get_x(square);
    int square_y = square->m->get_y(square);

    int x, y;
    for (y = -1; y <= 1; y++) {
        for (x = -1; x <= 1; x++) {
            Square *current = Map_get_square(map, square_x + x, square_y + y);
            surrounding_squares[y + 1][x + 1] = current;
        }
    }

    return surrounding_squares;
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
