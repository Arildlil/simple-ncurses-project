#include "test_headers.h"
#include "curses.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>



int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    Curses_init();

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
    return EXIT_SUCCESS;
}

