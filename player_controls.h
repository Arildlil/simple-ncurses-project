#ifndef PLAYER_CONTROLS_H
#define PLAYER_CONTROLS_H

#include "gameobject.h"

/*
 * Takes the next character typed in the input buffer
 * and decides what to do with it.
 * 
 * @arg input: The input data to use.
 * @arg hero: The player owned unit to manipulate.
 * @return: Whether the input was used or not.
 */
boolean PlayerControls_handle_input_char(char input, GameObject *hero);

#endif