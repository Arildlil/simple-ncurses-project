#ifndef TERRAIN_TYPES_H
#define TERRAIN_TYPES_H

#include "colors.h"

struct Image;

typedef struct TerrainType TerrainType;
typedef struct TerrainType_Methods TerrainType_Methods;

typedef enum TerrainType_Tag {
    TERRAIN_NONE, 
    TERRAIN_GRASS, 
    TERRAIN_WHEAT, 
    TERRAIN_WATER, 
    TERRAIN_STONE, 
    TERRAIN_TREE, 
    TERRAIN_ENUM_SIZE
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



extern TerrainType *terrain_default;
extern TerrainType_Methods terrain_methods;

extern Image TERRAIN_IMAGES[TERRAIN_ENUM_SIZE];
extern const char *TERRAIN_STRINGS[TERRAIN_ENUM_SIZE][5];

extern TerrainType terrain_types[TERRAIN_ENUM_SIZE];

/*
 * Initialization function for the TerrainTypes.
 */
void TerrainType_init();

#endif