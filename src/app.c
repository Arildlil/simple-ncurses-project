#include "tests/test_headers.h"
#include "include/curses.h"
#include "include/utils.h"
#include "include/gameobject.h"
#include "include/player_controls.h"
#include "include/player.h"
#include "include/map.h"
#include "include/rendering.h"
#include "include/unit_controllers.h"
#include "include/terrain_generator.h"
#include "include/gameobject_container.h"

#include "include/unit_defs.h"

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
static void init_world(int max_x_coord, int max_y_coord);
static void initialize_players();
static int generate_entities(unsigned int count);

static void Curses_redraw_objects(Map *map, GameObjectContainer *objects);
static void render_objects(Map *map, GameObjectContainer *objects);
static void cleanup(int sig);



/* ----- | Static Variables | ------ */

#define NO_GRAPHIC_MODE 0

#define FPS 10
#define US_PER_SEC 1000000
#define UPDATE_RATE_US (US_PER_SEC / FPS)

#define NUM_TROOPS 5
#define NUM_OBJECTS (NUM_TROOPS + 1)

static Player player, neutrals;

static GameObject_Controller player_controller;
static GameObject_Controller_Methods player_methods = {
    .on_tick = no_action,
    .shoot = peasant_shoot,
};

static int MIDDLE_X = 0;
static GameObject *hero;

static Map default_map;
//static GameObject *all_objects[NUM_OBJECTS];

static GameObjectContainer entities;




/* ----- | Functions | ----- */

int main(int argc, char *argv[]) {
    init();

    (void)argc;
    (void)argv;

    const int max_x_coord = 100;
    const int max_y_coord = 50;  
    
    GameObjectContainer_init(&entities, NUM_OBJECTS);
    
    assert(entities.m->get_size(&entities) > 0);

    init_world(max_x_coord, max_y_coord);
    generate_entities(5);
    initialize_players();

    MIDDLE_X = max_x / 2;

    int counter = 0;
    while (1) {
        /*
        int i;
        for (i = 0; i < NUM_TROOPS; i++) {
            GameObject *cur_object = all_objects[i];
            GameObject_Controller *cur_controller = cur_object->m->get_controller(cur_object);
            if (cur_controller != NULL) {
                cur_controller->m->on_tick(cur_controller, cur_object);
            }
        }
        */
        entities.m->on_tick(&entities);

        render_objects(&default_map, &entities);

        process_input();

        counter++;
        #ifdef DEBUG
        if (counter > 50)
            break;
        #endif
        usleep(UPDATE_RATE_US);
    }
    
    cleanup(OK);
}

static void init_world(int max_x_coord, int max_y_coord) {
    global_map = &default_map;
    Map_init(&default_map, max_x_coord, max_y_coord);
    TerrainGenerator_generate_default_map(&default_map);
}

static void initialize_players() {
    PlayerControls_init();

    Player_init(&player, COLOR_PAIR_RED, TRUE);
    Player_init(&neutrals, COLOR_PAIR_YELLOW, FALSE);

    GameObject_Controller_init(&player_controller, &player_methods);
    
    unsigned int inserted_index = 0;
    hero = entities.m->get_new_object(&entities, &inserted_index);
    Unit_new(&player, hero, &player_controller, "peasant", 0, 0);
}

static int generate_entities(unsigned int count) {
    assert(count <= NUM_TROOPS);
    
    int entities_generated = 0;
    
    char *units_to_spawn[] = {
        "archer",
        "swordman",
        "spearman",
    };
    
    unsigned int inserted_index, i;
    for (i = 0; i < count; i++) {
        GameObject *empty_gameobject = entities.m->get_new_object(&entities, &inserted_index);
        assert(empty_gameobject);
        
        Unit_new(&neutrals, empty_gameobject, NULL, units_to_spawn[0], MIDDLE_X, 10+5*i);
    }
    
    return entities_generated;
}

static void process_input() {
    char input = getch();

    if (input == ERR) {
        input = NO_KEYBOARD_INPUT;
    }

    PlayerControls_handle_input_char(input, hero);
}

static void render_objects(Map *map, GameObjectContainer *objects) {
    assert(objects != NULL);

    if (NO_GRAPHIC_MODE) {
        return;
    }
    
    Curses_redraw_objects(map, objects);
}

/*
 * Redraws the GameObjects.
 */
static void Curses_redraw_objects(Map *map, GameObjectContainer *objects) {
    getmaxyx(stdscr, max_y, max_x);
    clear();

    /* Draw the background Map. */
    unsigned int number_objects = 0;
    GameObject *all_objects = objects->m->get_all_objects(objects, &number_objects);
    
    Rendering_fill_framebuffer(map, hero->m->get_x(hero), hero->m->get_y(hero), 
        all_objects, number_objects);

    Rendering_render_frame();

    refresh();
}

/*
 * Initialization code.
 */
static int init() {
    srand(clock());
    //Rendering_init(40, 30);

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGKILL, cleanup);
    
    if (NO_GRAPHIC_MODE) {
        return OK;
    }
    
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
    dprintf("Cleaning up and exiting with sig %d...\n", sig);
    exit(0);
}
