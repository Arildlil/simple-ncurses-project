#include <unistd.h>

#include "utils.h"
#include "curses.h"


/* ----- | Static globals | ----- */

static boolean inited = FALSE;



/* ----- | Functions | ----- */

/*
 * Can be used to check if the 'ncurses' module
 * has been initialized.
 */
boolean Curses_is_inited(void) {
    return inited;
}

/*
 * Initializes ncurses.
 */
boolean Curses_init(void) {
    if (inited == FALSE) {
        dnprintf("Initializing ncurses...\n");
        
        inited = TRUE;
        //initscr();          /* Initialize the window */
        //noecho();           /* Don't echo any keypresses */
        //curs_set(FALSE);    /* Don't display a cursor */
    }
    return inited;
}

/*
 * Restore normal terminal behavior.
 */
boolean Curses_exit(void) {
    endwin();
    return TRUE;
}