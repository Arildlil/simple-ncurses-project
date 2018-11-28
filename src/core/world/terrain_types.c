#include "../../include/image.h"
#include "../../include/terrain_types.h"
#include "../../include/colors.h"



/* ----- | Prototypes | ------ */

static TerrainType_Tag TerrainType_get_tag(struct TerrainType *type);
static char *TerrainType_get_name(struct TerrainType *type);
static Image *TerrainType_get_image(struct TerrainType *type);
static Color_Pair TerrainType_get_colors(struct TerrainType *type);
static int TerrainType_get_width(struct TerrainType *type);
static int TerrainType_get_height(struct TerrainType *type);



/* ----- | Static Variables | ------ */

TerrainType_Methods terrain_methods = {
    .get_tag = TerrainType_get_tag,
    .get_name = TerrainType_get_name,
    .get_image = TerrainType_get_image,
    .get_colors = TerrainType_get_colors,
    .get_width = TerrainType_get_width,
    .get_height = TerrainType_get_height
};

Image TERRAIN_IMAGES[TERRAIN_ENUM_SIZE] = {0};
TerrainType *terrain_default = &terrain_types[TERRAIN_NONE];

const char *TERRAIN_STRINGS[TERRAIN_ENUM_SIZE][5] = {
    {
        ""
    },
    {
        " v^",
        "^v ",
        "v v",
    },
    {
        "v"
    },
    {
        "~"
    },
    {
        " OOO ",
        "OOOOO",
    },
    {
        "  A  ",
        " AAA ",
        "AAAAA",
        "AAAAA",
        " ||| ",
    },
};

TerrainType terrain_types[TERRAIN_ENUM_SIZE] = {
    {
        .tag = TERRAIN_NONE,
        .name = "none",
        .image = &TERRAIN_IMAGES[TERRAIN_NONE],
        .colors = COLOR_PAIR_NONE,
        .width = 1,
        .height = 1,
        .m = &terrain_methods,
    },
    {
        .tag = TERRAIN_GRASS,
        .name = "grass",
        .image = &TERRAIN_IMAGES[TERRAIN_GRASS],
        .colors = COLOR_PAIR_GRASS,
        .width = 3,
        .height = 3,
        .m = &terrain_methods,
    },
    {
        .tag = TERRAIN_WHEAT,
        .name = "wheat",
        .image = &TERRAIN_IMAGES[TERRAIN_WHEAT],
        .colors = COLOR_PAIR_WHEAT,
        .width = 1,
        .height = 1,
        .m = &terrain_methods,
    },
    {
        .tag = TERRAIN_WATER,
        .name = "water",
        .image = &TERRAIN_IMAGES[TERRAIN_WATER],
        .colors = COLOR_PAIR_WATER,
        .width = 1,
        .height = 1,
        .m = &terrain_methods,
    },
    {
        .tag = TERRAIN_STONE,
        .name = "stone",
        .image = &TERRAIN_IMAGES[TERRAIN_STONE],
        .colors = COLOR_PAIR_STONE,
        .width = 5,
        .height = 2,
        .m = &terrain_methods,
    },
    {
        .tag = TERRAIN_TREE,
        .name = "tree",
        .image = &TERRAIN_IMAGES[TERRAIN_TREE],
        .colors = COLOR_PAIR_TREE,
        .width = 5,
        .height = 5,
        .m = &terrain_methods,
    }
};



/* ----- | Functions | ----- */

void TerrainType_init() {    
    int tag;
    for (tag = TERRAIN_NONE; tag < TERRAIN_ENUM_SIZE; tag++) {
        Image_init_2D(&TERRAIN_IMAGES[tag], terrain_types[tag].width, 
            terrain_types[tag].height, TERRAIN_STRINGS[tag]);
    }
}

/* TerrainType */
static TerrainType_Tag TerrainType_get_tag(struct TerrainType *type) {
    return type->tag;
}

static char *TerrainType_get_name(struct TerrainType *type) {
    return type->name;
}

static Image *TerrainType_get_image(struct TerrainType *type) {
    return type->image;
}

static Color_Pair TerrainType_get_colors(struct TerrainType *type) {
    return type->colors;
}

static int TerrainType_get_width(struct TerrainType *type) {
    return type->width;
}

static int TerrainType_get_height(struct TerrainType *type) {
    return type->height;
}

