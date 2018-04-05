#ifndef TERRAIN_H
#define TERRAIN_H

#include "utils.h"
#include "curses.h"
#include "image.h"

typedef struct Square Square;
typedef struct Square_Methods Square_Methods;
typedef struct TerrainType TerrainType;
typedef struct TerrainType_Methods TerrainType_Methods;
typedef struct Map Map;
typedef struct Map_Methods Map_Methods;

typedef enum TerrainType_Tag {
    TERRAIN_NONE, TERRAIN_GRASS, TERRAIN_WHEAT, TERRAIN_WATER, TERRAIN_STONE, TERRAIN_TREE, TERRAIN_ENUM_SIZE
} TerrainType_Tag;

struct TerrainType {
    TerrainType_Tag tag;
    char *name;
    Image *image;
    Color_Pair colors;
    char **image_string;
    int width;
    int height;

    TerrainType_Methods *m;
};

struct TerrainType_Methods {
    TerrainType_Tag (*get_tag)(struct TerrainType *type);
    char *(*get_name)(struct TerrainType *type);
    Image *(*get_image)(struct TerrainType *type);
    Color_Pair (*get_colors)(struct TerrainType *type);
    int (*get_width)(struct TerrainType *type);
    int (*get_height)(struct TerrainType *type);
};

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

extern const TerrainType *terrain_default;
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