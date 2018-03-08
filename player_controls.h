#ifndef PLAYER_CONTROLS_H
#define PLAYER_CONTROLS_H

#include "gameobject.h"

/*
 * Takes the next character typed in the input buffer
 * and decides what to do with it.
 * 
 * @arg input: The input data to use.
 */
void PlayerControls_handle_input(char input, GameObject *hero);

#endif