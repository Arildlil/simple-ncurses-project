#include "curses.h"



/* ----- | Type Definitions | ----- */



static const Surface_options default_options = {
    .bounce = FALSE,
    .screen_only = TRUE,
};


/*
 * Initialize the Surface object.
 */
void Surface_init(Surface *surface, int x, int y, char *content, 
    Surface_options* options) {
    
    surface->x = x;
    surface->y = y;
    surface->content = content;

    if (options == NULL) {
        options = &default_options;
    }
    surface->options.bounce = options->bounce;
    surface->options.screen_only = options->screen_only;
}

/*
 * Move the Surface object.
 */
void Surface_move(Surface *surface, int x, int y) {
    surface->x += x;
    surface->y += y;

    if (surface->options.bounce == TRUE) {
        if (surface->x > max_x-1) {
            surface->x = max_x-1;
        }
        if (surface->y > max_y-1) {
            surface->y = max_y-1;
        }
    }
}