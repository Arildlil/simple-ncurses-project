#ifndef SURFACE_H
#define SURFACE_H

#include "utils.h"
#include "rect.h"
struct Image;

typedef enum {
    DEAD, ALIVE
} Surface_State;

typedef struct Surface_Options {
    /* If the object should bounce on edges. */
    boolean bounce;
    /* If the object is removed when leaving the screen. */
    boolean screen_only;
} Surface_Options;

typedef struct Surface {
    int x;
    int y;
    Rect rect; // To be handled!
    Image *image;
    Surface_Options options;
    Surface_State state;

    /* Surface methods. */
    void (*free)(struct Surface* surface);
    void (*movement)(struct Surface* surface, int x, int y);
    int (*get_x)(struct Surface* surface);
    int (*get_y)(struct Surface* surface);
    void (*set_xy)(struct Surface* surface, int x, int y);
    Image *(*get_image)(struct Surface* surface);
    Surface_State (*get_state)(struct Surface* surface);

    /* Surface->image methods. */
    int (*get_width)(struct Surface* image);
    int (*get_height)(struct Surface* image);
    char **(*get_pixels)(struct Surface* image);
} Surface;

#endif