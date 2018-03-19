#include "utils.h"
#include "image.h"
#include "rect.h"
#include "curses.h"

#include <curses.h>
#include <unistd.h>
#include <assert.h>




#define DEFAULT_DELAY 1000



static void define_color_pairs();



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
            define_color_pairs();    /* Define color pairs */
        }
        max_y = 0, max_x = 0;
        getmaxyx(stdscr, max_y, max_x);
    }
    return inited;
}

/*
 * Initialize color pairs 
 */
static void define_color_pairs() {
    init_pair(COLOR_PAIR_RED, COLOR_WHITE, COLOR_RED);
    //init_pair(COLOR_PAIR_RED, COLOR_RED, COLOR_WHITE);

    init_pair(COLOR_PAIR_GREEN, COLOR_WHITE, COLOR_GREEN);
    init_pair(COLOR_PAIR_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_PAIR_YELLOW, COLOR_WHITE, COLOR_YELLOW);

    init_pair(COLOR_PAIR_NONE, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_PAIR_GRASS, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_WATER, COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_PAIR_STONE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_TREE, COLOR_GREEN, COLOR_BLACK);
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