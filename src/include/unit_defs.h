#ifndef UNIT_DEFS_H
#define UNIT_DEFS_H

#define UNIT_IMAGE_WIDTH 3
#define UNIT_IMAGE_HEIGHT 3

#include "../include/gameobject.h"
#include "../player.h"


/*
 * Initializes the UnitDefs module.
 */
void UnitDefs_init();

/*
 * Creates and returns a new unit.
 * 
 * @arg owner: The owner of the new unit.
 * @arg name: Used to decide which unit to create.
 * @arg x: The x-coordinate of the new unit.
 * @arg y: The y-coordinate of the new unit.
 * @return: A new unit on success, NULL otherwise.
 */
GameObject *Unit_new(Player *owner, const char *name, int x, int y);

#endif