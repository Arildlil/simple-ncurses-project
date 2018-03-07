#include "utils.h"
#include "image.h"
#include "rect.h"
#include "curses.h"

#include <curses.h>
#include <unistd.h>
#include <assert.h>




#define DEFAULT_DELAY 1000

/* ----- | Definitions | ------ */





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
        initscr();          /* Initialize the window */
        noecho();           /* Don't echo any keypresses */
        cbreak();           /* Take input chars one at a time, no '\n' wait */
        // raw();              /* We don't want to have to press enter */
        curs_set(FALSE);    /* Don't display a cursor */
        keypad(stdscr, TRUE);   /* Enable keyboard mapping */
        nodelay(stdscr, TRUE);  /* Makes 'getch' no-blocking */
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
void Curses_redraw(Surface *surfaces[], int num_elements) {
    int i, j, k;
    getmaxyx(stdscr, max_y, max_x);
    clear();
    for (i = 0; i < num_elements; i++) {
        Surface* cur = surfaces[i];
        assert(cur != NULL);
        if (cur->state == DEAD) {
            continue;
        }
        
        char **pixels = cur->get_pixels(cur);
        assert(pixels != NULL);
        dprintf("Object <%d> (x,y)\n", i);
 
        for (j = 0; j < cur->get_height(cur); j++) {
            for (k = 0; k < cur->get_width(cur); k++) {
                char cur_pixel = pixels[j][k];
                dprintf("\t(%d,", cur->x+k);
                dprintf("%d) ", cur->y+j);
                mvaddch(cur->y+j, cur->x+k,cur_pixel);
            }
            dnprintf("\n");
        }
    }
    refresh();
}