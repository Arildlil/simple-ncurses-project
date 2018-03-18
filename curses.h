#ifndef CURSES_H
#define CURSES_H

#include "image.h"
#include "utils.h"
#include "rect.h"

typedef enum {
    COLOR_PAIR_RED = 1, 
    COLOR_PAIR_GREEN, 
    COLOR_PAIR_BLUE, 
    COLOR_PAIR_YELLOW,

    COLOR_PAIR_GRASS,
    COLOR_PAIR_WATER,
    COLOR_PAIR_STONE,
    COLOR_PAIR_TREE
} Color_Pair;

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

int max_x;
int max_y;

/*
 * Constructor for the 'Surface' type, where a single char is provided.
 * 
 * @arg Surface: The Surface to initialize.
 * @arg character: The graphical representation of the object.
 * @arg x: The initial x coordinate.
 * @arg y: The initial y coordinate.
 * @arg options (Optional): Options for how the Surface will 
 *  behave.
 * @return: The first argument is also returned.
 */
Surface* Surface_init_char(Surface *surface, char character, int x, int y,
    Surface_Options* options);

/*
 * Constructor for the 'Surface' type, where an Image is provided.
 * 
 * @arg Surface: The Surface to initialize.
 * @arg Image: The graphical representation of the object.
 * @arg x: The initial x coordinate.
 * @arg y: The initial y coordinate.
 * @arg options (Optional): Options for how the Surface will 
 *  behave.
 * @return: The first argument is also returned.
 */
Surface* Surface_init_image(Surface *surface, Image *image, int x, int y, 
    Surface_Options* options);

/*
 * Can be used to check if the 'ncurses' module
 * has been initialized yet.
 * 
 * @return: TRUE if 'Curses_init' has been called,
 *   FALSE otherwise.
 */
boolean Curses_is_inited(void);

/*
 * Initialize this module. Must be called first!
 * 
 * @return: TRUE on success, FALSE otherwise.
 */
boolean Curses_init(void);

/*
 * Restore normal terminal behavior.
 * 
 * @return: Should always return TRUE.
 */
boolean Curses_exit(void);

/*
 * Redraws the GameObjects.
 */
//void Curses_redraw_objects(GameObject *objects[], int num_elements);

/*
 * Redraws the screen.
 */
void Curses_redraw(Surface *surfaces[], int num_elements);

#endif