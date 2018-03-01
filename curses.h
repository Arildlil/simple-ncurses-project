#ifndef CURSES_H
#define CURSES_H

#include "utils.h"

typedef struct Surface {
    int x;
    int y;
    char *content;
} Surface;

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