#include "../../include/terrain_generator.h"



void TerrainGenerator_generate_default_map(Map *map) {
    int map_width = map->m->get_width(map);
    int map_height = map->m->get_height(map);
    (void)map_width;
    (void)map_height;
    /*int w, h;*/
    /*
    for (h = 0; h < map_height; h++) {
        for (w = 0; w < map_width; w++) {
            Square_init(map, w, h, TERRAIN_GRASS);
        }
    }*/

    int map_min_x = global_map->m->get_min_x(global_map);
    int map_min_y = global_map->m->get_min_y(global_map);
    int map_max_x = global_map->m->get_max_x(global_map);
    int map_max_y = global_map->m->get_max_y(global_map);

    /* Add some water. */
    enum {NUM_WATER_SEGMENTS = 6};
    Coordinates water_start[NUM_WATER_SEGMENTS] = 
        {
            {.x = map_min_x, .y = 35-21},
            {.x = 45-71, .y = 30-21},
            {.x = 46-71, .y = 30-21},
            {.x = 116-71, .y = 28-21},
            {.x = 123-71, .y = 15-21},
            {.x = 123-71, .y = 15-21},
        };
    Coordinates water_end[NUM_WATER_SEGMENTS] = 
        {
            {.x = 45-71, .y = 35-21},
            {.x = 46-71, .y = 35-21},
            {.x = 115-71, .y = 30-21},
            {.x = 130-71, .y = 32-21},
            {.x = 124-71, .y = 32-21},
            {.x = map_max_x, .y = 15-21},
        };
    int s, x, y;
    for (s = 0; s < NUM_WATER_SEGMENTS; s++) {
        for (y = water_start[s].y; y <= water_end[s].y; y++) {
            for (x = water_start[s].x; x <= water_end[s].x; x++) {
                //Square_init(map, x, y, TERRAIN_WATER);
                Map_set_square(map, x, y, TERRAIN_WATER);
            }
        }
    }

    /* Add some grass patches. */
    int patch_size_x = 3, patch_size_y = 2;
    int start_x = 15-71, start_y = 20-21;
    start_x -= start_x % SQUARE_SIZE;
    start_y -= start_y % SQUARE_SIZE;
    int j, k;
    for (k = start_y; k < (patch_size_y * SQUARE_SIZE) + start_y; k += SQUARE_SIZE) {
        for (j = start_x; j < (patch_size_x * SQUARE_SIZE) + start_x; j += SQUARE_SIZE) {
            //Square_init(map, j, k, TERRAIN_GRASS);
            Map_set_square(map, j, k, TERRAIN_GRASS);
        }
    }

    /* Add some terrain features for variation. */
    enum {NUM_TERRAIN_PIECES = 6};
    TerrainType_Tag tags[NUM_TERRAIN_PIECES] = {
        TERRAIN_TREE, TERRAIN_TREE, TERRAIN_STONE, TERRAIN_STONE, TERRAIN_TREE, TERRAIN_TREE
    };
    int x_coords[NUM_TERRAIN_PIECES] = {15-71, 25-71, 50-71, 65-71, 90-71, 135-71};
    int y_coords[NUM_TERRAIN_PIECES] = {4-21, 15-21, 7-21, 23-21, 6-21, 20-21};

    int i;
    for (i = 0; i < NUM_TERRAIN_PIECES; i++) {
        //Square_init(map, x_coords[i], y_coords[i], tags[i]);
        Map_set_square(map, x_coords[i], y_coords[i], tags[i]);
    }

    /* Add water to the borders around the map */
    for (i = map_min_x; i < map_max_x; i++) {
        //Square_init(map, i, map_min_y, TERRAIN_WATER);
        Map_set_square(map, i, map_min_y, TERRAIN_WATER);
    }

    for (i = map_min_x; i < map_max_x; i++) {
        //Square_init(map, i, map_max_y, TERRAIN_WATER);
        Map_set_square(map, i, map_max_y, TERRAIN_WATER);
    }

    for (j = map_min_y; j < map_max_y; j++) {
        //Square_init(map, map_min_x, j, TERRAIN_WATER);
        Map_set_square(map, map_min_x, j, TERRAIN_WATER);
    }

    for (j = map_min_y; j < map_max_y; j++) {
        //Square_init(map, map_max_x, j, TERRAIN_WATER);
        Map_set_square(map, map_max_x, j, TERRAIN_WATER);
    }
}