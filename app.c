#include "test_headers.h"
#include "curses.h"
#include "utils.h"
#include "unit_images.h"
#include "units.h"
#include "gameobject.h"
#include "units.h"
#include "player_controls.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#include <ncurses.h>



/* ----- | Prototypes | ------ */

static int init();
static void process_input();
static void render_objects(GameObject objects[], int num_objects);
static void render(Surface *surfaces[], int num_objects);
static void cleanup(int sig);

/* ----- | Static Variables | ------ */

static long update_rate_us = 50000;
static GameObject *hero;

/* ----- | Functions | ----- */



int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    init();

    (void)argc;
    (void)argv;

    char *units_to_spawn[] = {
        "archer",
        "archer",
        "swordman",
        "spearman"
    };

    #define NUM_TROOPS 4
    #define NUM_OBJECTS (NUM_TROOPS + 1)
    GameObject objects[NUM_OBJECTS];
    int i;
    for (i = 0; i < NUM_TROOPS; i++) {
        Units_init_name(&objects[i], 0, 10+5*i, units_to_spawn[i]);
    }
    /* Initialize a unit for the player to control */
    Units_init_archer(&objects[NUM_OBJECTS-1], 20, 20);
    hero = &objects[NUM_OBJECTS-1];

    int counter = 0;
    while (1) {
        render_objects(objects, NUM_OBJECTS);
        for (i = 0; i < NUM_TROOPS; i++) {
            objects[i].m->movement(&objects[i], 1, 0);
        }

        process_input();

        counter++;
        if (counter > 50)
            break;
        usleep(update_rate_us);
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
            PlayerControls_handle_input_char(input, hero);
            break;
    }
}

static void render_objects(GameObject objects[], int num_objects) {
    assert(objects != NULL);
    assert(num_objects >= 0);

    Surface *surfaces[num_objects];
    int i;
    for (i = 0; i < num_objects; i++) {
        surfaces[i] = objects[i].m->get_surface(&objects[i]);
    }
    render(surfaces, num_objects);
}

/* 
 * Render the specified objects.
 */
static void render(Surface *surfaces[], int num_objects) {
    assert(surfaces != NULL);
    assert(num_objects >= 0);

    Curses_redraw(surfaces, num_objects);
}

/*
 * Initialization code.
 */
static int init() {
    UnitImages_init();

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
    dprintf("Cleaning up and exiting with sig %d...\n", sig);
    exit(0);
}