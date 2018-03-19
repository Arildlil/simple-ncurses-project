#ifndef RESOURCES_UNITS_H
#define RESOURCES_UNITS_H

#include "gameobject.h"



/*
 * Allocates memory for a new unit, specified by 'name'.
 * 
 * @arg owner: The owning player.
 * @arg x: x-coordinate to spawn.
 * @arg y: y-coordinate to spawn.
 * @arg name: The name of the unit to spawn.
 * @return: A new unit if name is acceptable, NULL otherwise.
 */
GameObject *new_Unit(Player *owner, int x, int y, char *name);

/* 
 * Wrapper functions for the function above.
 */
GameObject *new_Archer(Player *owner, int x, int y);
GameObject *new_Swordman(Player *owner, int x, int y);
GameObject *new_Spearman(Player *owner, int x, int y);
GameObject *new_Peasant(Player *owner, int x, int y);

#endif