#include "utils.h"
#include "image.h"
#include "rect.h"
#include "curses.h"

#include <curses.h>
#include <unistd.h>




#define DEFAULT_DELAY 1000

/* ----- | Definitions | ------ */





/* ----- | Static globals | ----- */

static boolean inited = FALSE;

static int delay = DEFAULT_DELAY;



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
        initscr();          /* Initialize the window */
        noecho();           /* Don't echo any keypresses */
        cbreak();           /* Take input chars one at a time, no '\n' wait */
        curs_set(FALSE);    /* Don't display a cursor */
        keypad(stdscr, TRUE);   /* Enable keyboard mapping */
        if (has_colors()) {
            start_color();  /* GIFF NICE COLORS! */
        }
        max_y = 0, max_x = 0;
        getmaxyx(stdscr, max_y, max_x);
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

/*
 * Redraws the screen.
 */
void Curses_redraw(Surface surfaces[], int num_elements) {
    // int x = 0, y = 0;
    int i;
    getmaxyx(stdscr, max_y, max_x);
    clear();
    for (i = 0; i < num_elements; i++) {
        Surface* cur = &surfaces[i];
        if (cur->state == DEAD) {
            continue;
        } 
        //mvprintw(cur->y, cur->x, cur->content);
    }
    refresh();
}