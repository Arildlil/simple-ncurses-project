#include "../../include/utils.h"
#include "../../include/image.h"
#include "../../include/rect.h"
#include "../../include/curses.h"

#include <curses.h>
#include <unistd.h>
#include <assert.h>




#define DEFAULT_DELAY 1000



static void define_color_pairs();
static void draw_surface(Surface *surface);



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
    init_pair(COLOR_PAIR_GRASS, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_WHEAT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_WATER, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_PAIR_STONE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_TREE, COLOR_GREEN, COLOR_BLACK);

    init_pair(COLOR_PAIR_TEXT_BW, COLOR_WHITE, COLOR_BLACK);
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
    int i;
    getmaxyx(stdscr, max_y, max_x);
    clear();

    for (i = 0; i < num_elements; i++) {
        Surface* current_surface = surfaces[i];
        assert(current_surface != NULL);

        if (current_surface->state == DEAD) {
            continue;
        }
        
        draw_surface(current_surface);
    }

    refresh();
}

static void draw_surface(Surface *surface) {
    int j, k;

    char **pixels = surface->get_pixels(surface);
    assert(pixels != NULL);
    dprintf("Object <%d> (x,y)\n", i);

    for (j = 0; j < surface->get_height(surface); j++) {
        for (k = 0; k < surface->get_width(surface); k++) {
            char pixel = pixels[j][k];
            mvaddch(surface->y+j, surface->x+k, pixel);
            
            dprintf("\t(%d,", surface->x+k);
            dprintf("%d) ", surface->y+j);
        }
        
        dnprintf("\n");
    }
}
