#ifndef UNITS_H
#define UNITS_H

#include "gameobject.h"

/*
 * Creates and returns a new unit.
 * 
 * @arg object: The GameObject to initialize.
 * @arg owner: The Player that will own the unit.
 * @arg x: x-coordinate of the new unit.
 * @arg y: y-coordinate of the new unit.
 * @return: The new unit on success, NULL otherwise.
 */
GameObject *Units_init_archer(GameObject *object, Player *owner, int x, int y);
GameObject *Units_init_swordman(GameObject *object, Player *owner, int x, int y);
GameObject *Units_init_spearman(GameObject *object, Player *owner, int x, int y);
GameObject *Units_init_peasant(GameObject *object, Player *owner, int x, int y);

/* 
 * Like the functions above, except that 'unit_name' specifies which of 
 * the above that will be called.
 */
GameObject *Units_init_name(GameObject *object, Player *owner, int x, int y, char *unit_name);

#endif