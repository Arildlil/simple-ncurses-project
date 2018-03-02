#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "curses.h"



/* ----- | Type Definitions | ----- */



static const Surface_Options default_options = {
    .bounce = FALSE,
    .screen_only = TRUE,
};

/* ----- | Function Prototypes | ----- */

static void Surface_free(Surface *surface);
static void Surface_move(Surface *surface, int x, int y);
static int Surface_get_x(Surface *surface);
static int Surface_get_y(Surface *surface);
static Image *Surface_get_image(Surface *surface);

static boolean is_outside_screen(Surface *surface);



/*
 * Initialize the Surface object with a char.
 */
void Surface_init_char(Surface *surface, char character, int x, int y,
    Surface_Options* options) {

    char char_as_string[2] = {character, '\0'};
    Image image;
    Image_init_1D(&image, 1, char_as_string);
    Surface_init_image(surface, &image, x, y, options);
}

/*
 * Initialize the Surface object with an image.
 */
void Surface_init_image(Surface *surface, Image *image, int x, int y,
    Surface_Options* options) {
    
    if (surface == NULL) {
        surface->state = DEAD;
        return;
    }
    
    surface->x = x;
    surface->y = y;
    surface->image = calloc(x*y, sizeof(char));
    memcpy(&surface->image, image, x*y);
    surface->state = ALIVE;

    if (options == NULL) {
        options = &default_options;
    }
    surface->options.bounce = options->bounce;
    surface->options.screen_only = options->screen_only;

    surface->free = Surface_free;
    surface->move = Surface_move;
    surface->get_x = Surface_get_x;
    surface->get_y = Surface_get_y;
    surface->get_image = Surface_get_image;
}

static int Surface_get_x(Surface *surface) {
    return surface->x;
}

static int Surface_get_y(Surface *surface) {
    return surface->y;
}

static Image *Surface_get_image(Surface *surface) {
    return surface->image;
}

/*
 * Clean up the struct.
 */
static void Surface_free(Surface *surface) {
    surface->x = 0;
    surface->y = 0;
    free(surface->image);
    memcpy(&surface->options, &default_options, sizeof(default_options));
    surface->state = DEAD;
}

/*
 * Move the Surface object.
 */
static void Surface_move(Surface *surface, int x, int y) {
    int old_x = surface->x;
    int old_y = surface->y;
    
    surface->x += x;
    surface->y += y;

    /*
    if (surface->options.bounce == TRUE) {
        if (surface->x > max_x-1) {
            surface->x = max_x-1;
        }
        if (surface->y > max_y-1) {
            surface->y = max_y-1;
        }
    }*/

    /*
    if (is_outside_screen(surface)) {
        if (surface->options.screen_only) {
            Surface_remove(surface);
            return;
        }
    }*/
}

/*
 * Checks if the Surface is outside the screen.
 */
static boolean is_outside_screen(Surface *surface) {
    return (surface->x >= max_x || surface->y >= max_y ||
        surface->x < 0 || surface->y < 0);
}