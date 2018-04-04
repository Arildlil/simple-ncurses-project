#include "unit_controllers.h"
#include "terrain.h"
#include "utils.h"

#include <stdlib.h>



/* ----- | Prototypes | ------ */

static void default_on_tick(GameObject_Controller *controller, GameObject *object);
static boolean default_shoot(GameObject_Controller *controller, GameObject *object);
static boolean peasant_shoot(GameObject_Controller *controller, GameObject *object);



/* ----- | Static Variables | ------ */

static GameObject_Controller controller_archer;
static GameObject_Controller controller_swordman;
static GameObject_Controller controller_spearman;
static GameObject_Controller controller_peasant;

static GameObject_Controller_Methods controller_archer_methods = {
    .on_tick = default_on_tick,
    .shoot = default_shoot,
};

static GameObject_Controller_Methods controller_swordman_methods = {
    .on_tick = default_on_tick,
    .shoot = default_shoot,
};

static GameObject_Controller_Methods controller_spearman_methods = {
    .on_tick = default_on_tick,
    .shoot = default_shoot,
};

static GameObject_Controller_Methods controller_peasant_methods = {
    .on_tick = default_on_tick,
    .shoot = peasant_shoot,
};


static boolean inited = FALSE;

extern Map *global_map;



static void init(void) {
    inited = TRUE;

    GameObject_Controller_init(&controller_archer, &controller_archer_methods);
    GameObject_Controller_init(&controller_swordman, &controller_swordman_methods);
    GameObject_Controller_init(&controller_spearman, &controller_spearman_methods);
    GameObject_Controller_init(&controller_peasant, &controller_peasant_methods);
}

GameObject_Controller *get_controller_archer(void) {
    if (inited == FALSE) {
        init();
    }

    return &controller_archer;
}

GameObject_Controller *get_controller_swordman(void) {
    if (inited == FALSE) {
        init();
    }

    return &controller_swordman;
}

GameObject_Controller *get_controller_spearman(void) {
    if (inited == FALSE) {
        init();
    }

    return &controller_spearman;
}

GameObject_Controller *get_controller_peasant(void) {
    if (inited == FALSE) {
        init();
    }

    return &controller_peasant;
}



static void default_on_tick(GameObject_Controller *controller, GameObject *object) {
    (void)controller;

    if (object->m->get_order_count(object) > 0) {
        object->m->on_tick(object);
        return;
    }
    
    int direction_x = (rand() % 5) - 2;             /* -2 to 2 */
    int direction_y = (rand() % 5) - 2;             /* -2 to 2 */
    int movement_multiplier = (rand() % 2) + 2;     
    int movement_x = direction_x * MOVE_RANGE * movement_multiplier;
    int movement_y = direction_y * MOVE_RANGE * movement_multiplier;
    int new_x = object->m->get_x(object) + movement_x;
    int new_y = object->m->get_y(object) + movement_y;

    int object_height = object->m->get_height(object);

    int map_min_x = global_map->m->get_min_x(global_map);
    int map_min_y = global_map->m->get_min_y(global_map);
    int map_max_x = global_map->m->get_max_x(global_map);
    int map_max_y = global_map->m->get_max_y(global_map);
    if (new_x <= map_min_x) {
        new_x += map_min_x + movement_x * -1;
    } else if (new_y <= map_min_y) {
        new_y += map_min_y + movement_y * -1;
    } else if (new_x >= map_max_x - object_height) {
        new_x -= map_max_x - object_height - movement_x * -1;
    } else if (new_y >= map_max_y - object_height) {
        new_y -= map_max_y - object_height - movement_y * -1;
    }
    //new_x = MIN(MAX(new_x, 0), max_x - object_height);
    //new_y = MIN(MAX(new_y, 0), max_y - object_height);

    /*boolean result = FALSE;*/
    object->m->move_to(object, new_x, new_y, FALSE);
    object->m->on_tick(object);
}

static boolean default_shoot(GameObject_Controller *controller, GameObject *object) {
    (void)controller;
    (void)object;
    return TRUE;
}

static boolean peasant_shoot(GameObject_Controller *controller, GameObject *object) {
    (void)controller;
    (void)object;
    fprintf(stderr, "Get the hell off my lawn!\n");
    return TRUE;
}