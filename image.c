#include "image.h"

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
void Image_init_1D(Image *image, int width, char *content) {
    char *contentWrapper[1] = {content};
    Image_init_2D(image, width, 1, contentWrapper);
}

/*
 * Constructor for 2D images.
 */
void Image_init_2D(Image *image, int width, int height, char **content) {
    if (image == NULL) {
        return;
    }

    if (width < 0) width = 0;
    if (height < 0) height = 0;

    image->width = width;
    image->height = height;
    image->pixels = calloc(height, sizeof(char*));

    int i, j;
    for (i = 0; i < height; i++) {
        image->pixels[i] = calloc(width+1, sizeof(char));

        for (j = 0; j < width && content[i][j] != '\0'; j++) {
            image->pixels[i][j] = content[i][j];
        }
    }

    image->free = Image_free;
    image->get_height = Image_get_height;
    image->get_width = Image_get_width;
    image->get_pixels = Image_get_pixels;    
}

/*
 * Destructor for 1D images.
 */
static void Image_free(Image *image) {
    int i;
    for (i = 0; i < image->height; i++) {
        free(image->pixels[i]);
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

static char** Image_get_pixels(Image *image) {
    return image->pixels;
}