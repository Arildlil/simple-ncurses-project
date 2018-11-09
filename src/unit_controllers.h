#ifndef UNIT_CONTROLLERS_H
#define UNIT_CONTROLLERS_H

#include "gameobject.h"

#define MOVE_RANGE 5

GameObject_Controller *get_controller_archer(void);
GameObject_Controller *get_controller_swordman(void);
GameObject_Controller *get_controller_spearman(void);
GameObject_Controller *get_controller_peasant(void);

#endif