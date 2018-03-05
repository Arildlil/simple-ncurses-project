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
static void Surface_movement(Surface *surface, int x, int y);
static int Surface_get_x(Surface *surface);
static int Surface_get_y(Surface *surface);
static Image *Surface_get_image(Surface *surface);
static Surface_State Surface_get_state(Surface* surface);

static int Surface_get_width(Surface *surface);
static int Surface_get_height(Surface *surface);
static char **Surface_get_pixels(Surface *surface);

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
        return;
    }
    
    surface->x = x;
    surface->y = y;
    surface->image = calloc(1, sizeof(Image));
    memcpy(surface->image, image, sizeof(Image));
    surface->state = ALIVE;

    if (options == NULL) {
        options = &default_options;
    }
    surface->options.bounce = options->bounce;
    surface->options.screen_only = options->screen_only;

    surface->free = Surface_free;
    surface->movement = Surface_movement;
    surface->get_x = Surface_get_x;
    surface->get_y = Surface_get_y;
    surface->get_image = Surface_get_image;
    surface->get_state = Surface_get_state;

    surface->get_width = Surface_get_width;
    surface->get_height = Surface_get_height;
    surface->get_pixels = Surface_get_pixels;
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

static Surface_State Surface_get_state(Surface* surface) {
    return surface->state;
}

/*
 * Clean up the struct.
 */
static void Surface_free(Surface *surface) {
    surface->x = 0;
    surface->y = 0;
    if (surface->image != NULL) {
        surface->image->free(surface->image);
        free(surface->image);
    }
    surface->image = NULL;
    memcpy(&surface->options, &default_options, sizeof(default_options));
    surface->state = DEAD;
}

/*
 * Move the Surface object.
 */
static void Surface_movement(Surface *surface, int x, int y) {
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

/* Image related */

static int Surface_get_width(Surface *surface) {
    return surface->image->get_width(surface->image);
}

static int Surface_get_height(Surface *surface) {
    return surface->image->get_height(surface->image);
}

static char **Surface_get_pixels(Surface *surface) {
    return surface->image->get_pixels(surface->image);
}


/* Other */

/*
 * Checks if the Surface is outside the screen.
 */
static boolean is_outside_screen(Surface *surface) {
    return (surface->x >= max_x || surface->y >= max_y ||
        surface->x < 0 || surface->y < 0);
}