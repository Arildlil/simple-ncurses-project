#include "test_headers.h"
#include "curses.h"
#include "utils.h"
#include "unit_images.h"
#include "units.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#include <ncurses.h>



/* ----- | Prototypes | ------ */

static int init();
static void render(Surface objects[], int num_objects);
static void cleanup(int sig);

/* ----- | Static Variables | ------ */

static long update_rate_us = 45000;

/* ----- | Functions | ----- */



int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    init();

    (void)argc;
    (void)argv;

    int i;
    Image *unit_images[] = {
        &UNIT_IMAGE_ARCHER, &UNIT_IMAGE_ARCHER, &UNIT_IMAGE_SWORDMAN, &UNIT_IMAGE_SPEARMAN
    };

    #define NUM_OBJECTS 4
    Surface objects[NUM_OBJECTS];
    for (i = 0; i < NUM_OBJECTS; i++) {
        Surface_init_image(&objects[i], unit_images[i], 0, 10+5*i, NULL);
    }

    int counter = 0;
    while (1) {
        render(objects, NUM_OBJECTS);
        for (i = 0; i < NUM_OBJECTS; i++) {
            objects[i].movement(&objects[i], 1, 0);
        }
        counter++;
        if (counter > 50)
            break;
    }
    printf("max_x: %d, max_y: %d\n", max_x, max_y);
    
    cleanup(OK);
}

/* 
 * Render the specified objects.
 */
static void render(Surface objects[], int num_objects) {
    assert(objects != NULL);
    assert(num_objects >= 0);

    Curses_redraw(objects, num_objects);
    usleep(update_rate_us);
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