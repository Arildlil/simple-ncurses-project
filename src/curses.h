#ifndef CURSES_H
#define CURSES_H

#include "colors.h"
#include "utils.h"
#include "image.h"
#include "surface.h"

struct Surface;
struct Image;

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