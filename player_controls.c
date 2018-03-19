#include "player_controls.h"
#include "utils.h"

#include <ctype.h>



/* ----- | Static Variables | ------ */

enum {
    MOVEMENT_PER_UPDATE = 3,

    MOVE_UP = 'w',
    MOVE_DOWN = 's',
    MOVE_RIGHT = 'd',
    MOVE_LEFT = 'a',
    MOVE_UP_LEFT = 'q',
    MOVE_UP_RIGHT = 'e',
    MOVE_DOWN_LEFT = '<',
    MOVE_DOWN_RIGHT = 'x',

    ATTACK = ' ',
};



/* ----- | Functions | ----- */

void PlayerControls_init() {
    
}



boolean PlayerControls_handle_input_char(char input, GameObject *hero) {
    if (hero == NULL) {
        return FALSE;
    }
    input = tolower(input);

    switch (input) {
        case MOVE_UP:
            hero->m->movement(hero, 0, -MOVEMENT_PER_UPDATE);
            break;
        case MOVE_DOWN:
            hero->m->movement(hero, 0, MOVEMENT_PER_UPDATE);
            break;
        case MOVE_RIGHT: 
            hero->m->movement(hero, MOVEMENT_PER_UPDATE, 0);
            break;
        case MOVE_LEFT:
            hero->m->movement(hero, -MOVEMENT_PER_UPDATE, 0);
            break;
        case MOVE_UP_LEFT:
            hero->m->movement(hero, -MOVEMENT_PER_UPDATE, -MOVEMENT_PER_UPDATE);
            break;
        case MOVE_UP_RIGHT:
            hero->m->movement(hero, MOVEMENT_PER_UPDATE, -MOVEMENT_PER_UPDATE);
            break;
        case MOVE_DOWN_LEFT:
            hero->m->movement(hero, -MOVEMENT_PER_UPDATE, MOVEMENT_PER_UPDATE);
            break;
        case MOVE_DOWN_RIGHT:
            hero->m->movement(hero, MOVEMENT_PER_UPDATE, MOVEMENT_PER_UPDATE);
            break;
        case ATTACK:
            hero->m->shoot(hero);
        default:
            return FALSE;
    }

    return TRUE;
}