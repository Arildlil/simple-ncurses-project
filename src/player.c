#include "player.h"
#include "utils.h"



/* ----- | Prototypes | ------ */

static int Player_get_player_id(struct Player *player);
static Color_Pair Player_get_colors(struct Player *player);
static boolean Player_is_real_player(struct Player *player);



/* ----- | Static Variables | ------ */

static Player_Methods methods = {
    .get_player_id = Player_get_player_id,
    .get_colors = Player_get_colors,
    .is_real_player = Player_is_real_player
};

static int next_player_id = 0;

Player dummy_player = {
    .player_id = 0,
    .colors = COLOR_PAIR_GREEN,
    .real_player = FALSE,

    .m = &methods
};



/* ----- | Functions | ------ */



Player *Player_init(Player *player, Color_Pair color, boolean real_player) {
    if (player == NULL) {
        return NULL;
    }

    player->player_id = next_player_id++;
    player->colors = color;
    player->real_player = real_player;

    player->m = &methods;
    
    return player;    
}

static int Player_get_player_id(struct Player *player) {
    return player->player_id;
}

static Color_Pair Player_get_colors(struct Player *player) {
    return player->colors;
}

static boolean Player_is_real_player(struct Player *player) {
    return player->real_player;
}