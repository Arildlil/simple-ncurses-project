#include "test_headers.h"
#include "curses.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <ncurses.h>



/* ----- | Prototypes | ------ */

static void cleanup(int sig);



/* ----- | Functions | ----- */

/*
 * Initialization code.
 */
static int init() {
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
    Curses_exit();
    sleep(1);
    dnprintf("Cleaning up and exiting...\n");
    exit(0);
}



int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    init();

    #define NUM_OBJECTS 2
    Surface objects[NUM_OBJECTS];
    Surface_init(&objects[0], 0, 0, "o", NULL);
    Surface_options options = {.bounce = TRUE};
    Surface_init(&objects[1], 25, 0, "*", &options);

    int counter = 0;
    while (1) {
        Curses_redraw(objects, NUM_OBJECTS);
        usleep(30000);
        Surface_move(&objects[0], 1, 1);
        Surface_move(&objects[1], 1, 1);
        /*counter++;
        if (counter > 50)
            break;*/
    }
    Curses_exit();
    printf("max_x: %d, max_y: %d\n", max_x, max_y);
    
    cleanup(OK);
}

