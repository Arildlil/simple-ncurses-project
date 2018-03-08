#include "player_controls.h"
#include "utils.h"

#include <ctype.h>



/* ----- | Static Variables | ------ */

#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_RIGHT 'd'
#define MOVE_LEFT 'a'


/* ----- | Functions | ----- */



boolean PlayerControls_handle_input_char(char input, GameObject *hero) {
    if (hero == NULL) {
        return FALSE;
    }
    input = tolower(input);

    switch (input) {
        case MOVE_UP:
            hero->m->movement(hero, 0, -1);
            break;
        case MOVE_DOWN:
            hero->m->movement(hero, 0, 1);
            break;
        case MOVE_RIGHT: 
            hero->m->movement(hero, 1, 0);
            break;
        case MOVE_LEFT:
            hero->m->movement(hero, -1, 0);
            break;
        default:

            return FALSE;
    }

    return TRUE;
}