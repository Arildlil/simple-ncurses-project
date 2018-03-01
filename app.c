
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "test_headers.h"
#include "curses.h"
#include "utils.h"



int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    Curses_init();
    Curses_init();
    Curses_init();
    return EXIT_SUCCESS;
}

