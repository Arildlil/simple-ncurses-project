#ifndef UNIT_CONTROLLERS_H
#define UNIT_CONTROLLERS_H

#include "../include/gameobject.h"

#define MOVE_RANGE 5

GameObject_Controller *get_controller(const char *name);

#endif