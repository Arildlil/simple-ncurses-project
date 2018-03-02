#ifndef IMAGE_H
#define IMAGE_H

typedef struct Image {
    int width;
    int height;
    char **pixels;

    void (*free)(struct Image* image);
    int (*get_width)(struct Image* image);
    int (*get_height)(struct Image* image);
    char **(*get_pixels)(struct Image* image);
} Image;

/*
 * Constructor for 1D images.
 * 
 * @arg image: The image to initialize.
 * @arg width: The width of the image.
 * @arg content: The string representation of the image.
 */
void Image_init_1D(Image *image, int width, char *content);

/*
 * Constructor for 2D images.
 * 
 * @arg image: The image to initialize.
 * @arg width: The width of the image.
 * @arg height: The height of the image.
 * @arg content: The 2D string representation of the image.
 */
void Image_init_2D(Image *image, int width, int height, char **content);

#endif