#ifndef CURSES_H
#define CURSES_H

#include "image.h"
#include "utils.h"

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
    char *content;
    Surface_Options options;
    Surface_State state;
} Surface;

int max_x;
int max_y;

/*
 * Construction for the 'Surface' type.
 * 
 * @arg Surface: The Surface to initialize.
 * @arg x: The initial x coordinate.
 * @arg y: The initial y coordinate.
 * @arg content: The graphical representation of the object.
 * @arg options (Optional): Options for how the Surface will 
 *  behave.
 */
void Surface_init(Surface *surface, int x, int y, char *content, 
    Surface_Options* options);

/*
 * Moves the 'Surface' by the specified arguments.
 * 
 * @arg Surface: The Surface to initialize.
 * @arg x: The initial x coordinate.
 * @arg y: The initial y coordinate.
 */
void Surface_move(Surface *surface, int x, int y);

/*
 * Cleans up the Surface and marks it as unused.
 * 
 * @arg Surface: The Surface to clean up.
 */
void Surface_remove(Surface *surface);

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
 * Redraws the screen.
 */
void Curses_redraw(Surface surfaces[], int num_elements);

#endif