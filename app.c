#include "test_headers.h"
#include "curses.h"
#include "utils.h"
#include "unit_images.h"
#include "units.h"
#include "gameobject.h"
#include "units.h"
#include "player_controls.h"
#include "player.h"
#include "terrain.h"
#include "resources.h"
#include "resources_units.h"
#include "rendering.h"
#include "unit_controllers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <assert.h>
#include <time.h>
#include <sys/param.h>

#include <ncurses.h>



/* ----- | Prototypes | ------ */

static int init();
static void process_input();
static void Curses_redraw_objects(Map *map, GameObject *objects[], int num_elements);
static void render_objects(Map *map, GameObject *objects[], int num_objects);
static void cleanup(int sig);



/* ----- | Static Variables | ------ */

#define FPS 10
#define US_PER_SEC 1000000
#define UPDATE_RATE_US (US_PER_SEC / FPS)

static int MIDDLE_X = 0;
static GameObject *hero;



/* ----- | Functions | ----- */

static void generate_default_map(Map *map) {
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
                Square_init(map, x, y, TERRAIN_WATER);
            }
        }
    }

    /* Add some grass patches. */
    int patch_size_x = 6, patch_size_y = 4;
    int start_x = 15-71, start_y = 20-21;
    int j, k;
    for (k = start_y; k < patch_size_y + start_y; k++) {
        for (j = start_x; j < patch_size_x + start_x; j++) {
            Square_init(map, j, k, TERRAIN_GRASS);
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
        Square_init(map, x_coords[i], y_coords[i], tags[i]);
    }

    /* Add water to the borders around the map */
    for (i = map_min_x; i < map_max_x; i++) {
        Square_init(map, i, map_min_y, TERRAIN_WATER);
    }

    for (i = map_min_x; i < map_max_x; i++) {
        Square_init(map, i, map_max_y, TERRAIN_WATER);
    }

    for (j = map_min_y; j < map_max_y; j++) {
        Square_init(map, map_min_x, j, TERRAIN_WATER);
    }

    for (j = map_min_y; j < map_max_y; j++) {
        Square_init(map, map_max_x, j, TERRAIN_WATER);
    }
}

int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    init();

    (void)argc;
    (void)argv;

    Player player, neutrals;
    Player_init(&player, COLOR_PAIR_RED, TRUE);
    Player_init(&neutrals, COLOR_PAIR_YELLOW, FALSE);

    Map default_map;
    global_map = &default_map;
    Map_init(&default_map, 100, 50);
    generate_default_map(&default_map);

    char *units_to_spawn[] = {
        "archer",
        "archer",
        "swordman",
        "spearman",
        "peasant",
    };

    PlayerControls_init();

    GameObject_Controller player_controller;
    GameObject_Controller_Methods player_methods = {
        .on_tick = NULL,
        .shoot = get_controller_peasant()->m->shoot,
    };
    GameObject_Controller_init(&player_controller, &player_methods);

    MIDDLE_X = max_x / 2;
    #define NUM_TROOPS 5
    #define NUM_OBJECTS (NUM_TROOPS + 1)

    GameObject *all_objects[NUM_OBJECTS];
    
    int i;
    for (i = 0; i < NUM_TROOPS; i++) {
        all_objects[i] = new_Unit(&neutrals, MIDDLE_X, 10+5*i, units_to_spawn[i]);
    }

    hero = new_Unit(&player, 0, 0, "peasant");
    all_objects[NUM_OBJECTS-1] = hero;
    hero->m->set_controller(hero, &player_controller);

    int counter = 0;
    while (1) {
        Resources_on_tick();
        render_objects(&default_map, all_objects, NUM_OBJECTS);

        process_input();

        counter++;
        #ifdef DEBUG
        if (counter > 50)
            break;
        #endif
        usleep(UPDATE_RATE_US);
    }
    printf("max_x: %d, max_y: %d\n", max_x, max_y);
    
    cleanup(OK);
}

static void process_input() {
    char input = getch();

    switch (input) {
        case ERR: 
            break;
        case 'a':
            /* Intentional fall-through */
        case 'd':
            /* Intentional fall-through */
        case 'w':
            /* Intentional fall-through */
        case 's':
            /* Intentional fall-through */
        case 'q':
            /* Intentional fall-through */
        case 'e':
            /* Intentional fall-through */
        case '<':
            /* Intentional fall-through */
        case ' ':
            /* Intentional fall-through */
        case 'x':
            PlayerControls_handle_input_char(input, hero);
            break;
    }
}

static void render_objects(Map *map, GameObject *objects[], int num_objects) {
    assert(objects != NULL);
    assert(num_objects >= 0);

    Curses_redraw_objects(map, objects, num_objects);
}

/*
 * Redraws the GameObjects.
 */
static void Curses_redraw_objects(Map *map, GameObject *objects[], int num_elements) {
    getmaxyx(stdscr, max_y, max_x);
    clear();

    /* Draw the background Map. */
    Rendering_fill_framebuffer(map, hero->m->get_x(hero), hero->m->get_y(hero), objects, num_elements);

    Rendering_render_frame();

    refresh();
}

/*
 * Initialization code.
 */
static int init() {
    UnitImages_init();
    srand(clock());
    Resources_init(DEFAULT_MAX_GAMEOBJECT_COUNT);
    //Rendering_init(40, 30);

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGKILL, cleanup);
    Curses_init();
    if (Rendering_init(max_x, max_y) == FALSE) {
        cleanup(0);
    }
    return OK;
}

/*
 * Clean up before exiting.
 * 
 * @arg sig: Signal that caused this to be called. 
 *   0 on normal termination.
 */
static void cleanup(int sig) {
    (void)sig;
    Curses_exit();
    Resources_exit();
    dprintf("Cleaning up and exiting with sig %d...\n", sig);
    exit(0);
}