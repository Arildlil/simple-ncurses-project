#ifndef PLAYER_H
#define PLAYER_H

#include "curses.h"

/* Forward declaration */
typedef struct Player_Methods Player_Methods;

typedef struct Player {
    int player_id;
    Color_Pair colors;
    boolean real_player;

    /* Common methods for all GameObjects */
    Player_Methods *m;
} Player;

struct Player_Methods {
    int (*get_player_id)(struct Player *player);
    Color_Pair (*get_colors)(struct Player *player);
    boolean (*is_real_player)(struct Player *player);
};

/*
 * Constructor for Players.
 * 
 * @arg player: The struct to initialize
 * @arg color: The color pair to use for the player units.
 * @arg real_player: Whether the player is player controlled or not.
 * @return: The player argument.
 */
Player *Player_init(Player *player, Color_Pair color, boolean real_player);

#endif