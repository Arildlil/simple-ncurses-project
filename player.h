#ifndef PLAYER_H
#define PLAYER_H

/* Forward declaration */
typedef struct Player_Methods Player_Methods;

typedef struct Player {
    int player_id;

    /* Common methods for all GameObjects */
    Player_Methods *m;
} Player;

typedef struct Player_Methods {
    int (*get_player_id)(struct Player *player);
};

/*
 * Constructor for Players.
 * 
 * @arg
 */
Player *Player_init(Player *player);

#endif