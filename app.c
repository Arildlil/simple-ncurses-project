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
#define MOVE_RANGE 5
static int MIDDLE_X = 0;
static GameObject *hero;



/* ----- | Functions | ----- */

static void default_on_tick(GameObject_Controller *controller, GameObject *object) {
    (void)controller;

    if (object->m->get_order_count(object) > 0) {
        object->m->on_tick(object);
        return;
    }
    
    int direction_x = (rand() % 5) - 2;             /* -2 to 2 */
    int direction_y = (rand() % 5) - 2;             /* -2 to 2 */
    int movement_multiplier = (rand() % 2) + 2;     
    int movement_x = direction_x * MOVE_RANGE * movement_multiplier;
    int movement_y = direction_y * MOVE_RANGE * movement_multiplier;
    int new_x = object->m->get_x(object) + movement_x;
    int new_y = object->m->get_y(object) + movement_y;

    int object_height = object->m->get_height(object);

    if (new_x <= object_height) {
        new_x += object_height + movement_x * -1;
    } else if (new_y <= object_height) {
        new_y += object_height + movement_y * -1;
    } else if (new_x >= max_x - object_height) {
        new_x -= max_x - object_height - movement_x * -1;;
    } else if (new_y >= max_y - object_height) {
        new_y -= max_y - object_height - movement_y * -1;;
    }
    new_x = MIN(MAX(new_x, 0), max_x - object_height);
    new_y = MIN(MAX(new_y, 0), max_y - object_height);

    /*boolean result = FALSE;*/
    object->m->move_to(object, new_x, new_y, FALSE);
    object->m->on_tick(object);
}

static boolean default_shoot(GameObject_Controller *controller, GameObject *object) {
    (void)controller;
    (void)object;
    return TRUE;
}

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

    /* Add some water. */
    enum {NUM_WATER_SEGMENTS = 6};
    Coordinates water_start[NUM_WATER_SEGMENTS] = 
        {
            {.x = 0, .y = 35},
            {.x = 45, .y = 30},
            {.x = 46, .y = 30},
            {.x = 116, .y = 28},
            {.x = 123, .y = 15},
            {.x = 123, .y = 15},
        };
    Coordinates water_end[NUM_WATER_SEGMENTS] = 
        {
            {.x = 45, .y = 35},
            {.x = 46, .y = 35},
            {.x = 115, .y = 30},
            {.x = 130, .y = 32},
            {.x = 124, .y = 32},
            {.x = 145, .y = 15},
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
    int start_x = 15, start_y = 20;
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
    int x_coords[NUM_TERRAIN_PIECES] = {15, 25, 50, 65, 90, 135};
    int y_coords[NUM_TERRAIN_PIECES] = {4, 15, 7, 23, 6, 20};

    int i;
    for (i = 0; i < NUM_TERRAIN_PIECES; i++) {
        Square_init(map, x_coords[i], y_coords[i], tags[i]);
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
    Map_init(&default_map, max_x, max_y);
    generate_default_map(&default_map);

    char *units_to_spawn[] = {
        "archer",
        "archer",
        "swordman",
        "spearman",
        "peasant",
    };

    GameObject_Controller random_controller;
    GameObject_Controller_Methods random_methods = {
        .on_tick = default_on_tick,
        .shoot = default_shoot,
    };
    GameObject_Controller_init(&random_controller, &random_methods);
    PlayerControls_init();

    MIDDLE_X = max_x / 2;
    #define NUM_TROOPS 5
    #define NUM_OBJECTS (NUM_TROOPS + 1)

    GameObject *all_objects[NUM_OBJECTS];
    
    int i;
    for (i = 0; i < NUM_TROOPS; i++) {
        all_objects[i] = new_Unit(&neutrals, MIDDLE_X, 10+5*i, units_to_spawn[i]);
        all_objects[i]->m->set_controller(all_objects[i], &random_controller);
    }

    hero = new_Unit(&player, 20, 20, "peasant");
    all_objects[NUM_OBJECTS-1] = hero;

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
    Rendering_add_background(map, hero->m->get_x(hero), hero->m->get_y(hero));

    /* Draw the GameObjects. */
    //Rendering_for_each(Rendering_render_object, RESOURCE_ALL);

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
    Rendering_init(max_x, max_y);

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGKILL, cleanup);
    Curses_init();
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