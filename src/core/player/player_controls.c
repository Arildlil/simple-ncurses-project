#include "../../include/player_controls.h"
#include "../../include/utils.h"
#include "../../include/game_log.h"

#include <ctype.h>
#include <assert.h>



static void set_movement_x_y(int x, int y);



/* ----- | Static Variables | ------ */

enum {
    MOVE_UP = 'w',
    MOVE_DOWN = 's',
    MOVE_RIGHT = 'd',
    MOVE_LEFT = 'a',
    MOVE_UP_LEFT = 'q',
    MOVE_UP_RIGHT = 'e',
    MOVE_DOWN_LEFT = '<',
    MOVE_DOWN_RIGHT = 'x',

    STOP = 'z',
    ATTACK = ' ',
};

static int per_tick_movement_x = 0;
static int per_tick_movement_y = 0;



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
            set_movement_x_y(0, -1);
            break;
        case MOVE_DOWN:
            set_movement_x_y(0, 1);
            break;
        case MOVE_RIGHT: 
            set_movement_x_y(1, 0);
            break;
        case MOVE_LEFT:
            set_movement_x_y(-1, 0);
            break;
        case MOVE_UP_LEFT:
            set_movement_x_y(-1, -1);
            break;
        case MOVE_UP_RIGHT:
            set_movement_x_y(1, -1);
            break;
        case MOVE_DOWN_LEFT:
            set_movement_x_y(-1, 1);
            break;
        case MOVE_DOWN_RIGHT:
            set_movement_x_y(1, 1);
            break;
        case STOP:
            set_movement_x_y(0, 0);
            break;
        case ATTACK:
            hero->m->shoot(hero);
            break;
        case NO_KEYBOARD_INPUT:
            break;
        default:
            return FALSE;
    }

    hero->m->movement(hero, per_tick_movement_x, per_tick_movement_y);

    return TRUE;
}

static void set_movement_x_y(int x, int y) {
    assert(x <= 1 && x >= -1);
    assert(y <= 1 && y >= -1);

    per_tick_movement_x = x;
    per_tick_movement_y = y;
}

