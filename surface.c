#include <stdlib.h>
#include <string.h>

#include "curses.h"



/* ----- | Type Definitions | ----- */



static const Surface_Options default_options = {
    .bounce = FALSE,
    .screen_only = TRUE,
};

/* ----- | Function Prototypes | ----- */
static boolean is_outside_screen(Surface *surface);


/*
 * Initialize the Surface object.
 */
void Surface_init(Surface *surface, int x, int y, char *content, 
    Surface_Options* options) {
    
    surface->x = x;
    surface->y = y;
    surface->content = content;
    surface->state = ALIVE;

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

    if (is_outside_screen(surface)) {
        if (surface->options.screen_only) {
            Surface_remove(surface);
            return;
        }
    }
}

/*
 * Clean up the struct.
 */
void Surface_remove(Surface *surface) {
    surface->x = 0;
    surface->y = 0;
    surface->content = NULL;
    memcpy(&surface->options, &default_options, sizeof(default_options));
    surface->state = DEAD;
}

/*
 * Checks if the Surface is outside the screen.
 */
static boolean is_outside_screen(Surface *surface) {
    return (surface->x >= max_x || surface->y >= max_y ||
        surface->x < 0 || surface->y < 0);
}