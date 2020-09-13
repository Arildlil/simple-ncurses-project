#ifndef UNIT_CONTROLLERS_H
#define UNIT_CONTROLLERS_H

#include "../include/gameobject.h"

#define MOVE_RANGE 5

GameObject_Controller *get_controller(const char *name);

void no_action(GameObject_Controller *controller, GameObject *object);
void default_on_tick(GameObject_Controller *controller, GameObject *object);
boolean default_shoot(GameObject_Controller *controller, GameObject *object);
boolean peasant_shoot(GameObject_Controller *controller, GameObject *object);

boolean farm_square(GameObject *object);

#endif
