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
#include "terrain_generator.h"

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
    TerrainGenerator_generate_default_map(&default_map);

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