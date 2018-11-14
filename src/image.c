#include "include/image.h"

#include <stdlib.h>
#include <string.h>



/* ----- | Function Prototypes | ----- */

static void Image_free(Image*);
static int Image_get_height(Image*);
static int Image_get_width(Image*);
static char **Image_get_pixels(Image*);



/*
 * Constructor for 1D images.
 */
Image* Image_init_1D(Image *image, int width, const char *content) {
    const char *contentWrapper[1] = {content};
    return Image_init_2D(image, width, 1, (const char**)contentWrapper);
}

/*
 * Constructor for 2D images.
 */
Image* Image_init_2D(Image *image, int width, int height, const char **content) {
    if (image == NULL) {
        return NULL;
    }

    if (width < 0) width = 0;
    if (height < 0) height = 0;

    image->width = width;
    image->height = height;
    image->pixels = calloc(height, sizeof(char*));

    int i;
    for (i = 0; i < height; i++) {
        image->pixels[i] = calloc(width+1, sizeof(char));

        strncpy(image->pixels[i], content[i], width);
        image->pixels[i][width] = '\0';
    }

    image->free = Image_free;
    image->get_height = Image_get_height;
    image->get_width = Image_get_width;
    image->get_pixels = Image_get_pixels;    

    return image;
}

/*
 * Destructor for 1D images.
 */
static void Image_free(Image *image) {
    int i;
    for (i = 0; i < image->height; i++) {
        if (image->pixels[i] != NULL) {
            free(image->pixels[i]);
        }
        image->pixels[i] = NULL;
    }
    free(image->pixels);
    image->pixels = NULL;

    image->width = 0;
    image->height = 0;
}

static int Image_get_height(Image *image) {
    return image->height;
}

static int Image_get_width(Image *image) {
    return image->width;
}

static char **Image_get_pixels(Image *image) {
    return image->pixels;
}