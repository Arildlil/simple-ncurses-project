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

    Surface objects[] = {
        {0,0,"o"},
    };
    while (1) {
        Curses_redraw(objects, 1);
        usleep(100000);
        Surface* o = &objects[0];
        o->x++;
        o->y++;
        if (o->x > 40 || o->y > 40) {
            break;
        }
    }
    Curses_exit();
    return EXIT_SUCCESS;
}

