#include "../include/unit_controllers.h"
#include "../include/map.h"
#include "../include/utils.h"
#include "../include/game_log.h"

#include <stdlib.h>



/* ----- | Prototypes | ------ */





/* ----- | Static Variables | ------ */

static GameObject_Controller controller_default;
static GameObject_Controller controller_archer;
static GameObject_Controller controller_swordman;
static GameObject_Controller controller_spearman;
static GameObject_Controller controller_peasant;

static GameObject_Controller_Methods controller_default_methods = {
    .on_tick = default_on_tick,
    .shoot = default_shoot,
};

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

    GameObject_Controller_init(&controller_default, &controller_default_methods);
    GameObject_Controller_init(&controller_archer, &controller_archer_methods);
    GameObject_Controller_init(&controller_swordman, &controller_swordman_methods);
    GameObject_Controller_init(&controller_spearman, &controller_spearman_methods);
    GameObject_Controller_init(&controller_peasant, &controller_peasant_methods);
}

GameObject_Controller *get_controller(const char *name) {
    if (inited == FALSE) {
        init();
    }

    if (StrRel(name, ==, "archer")) {
        return &controller_archer;
    } 
    if (StrRel(name, ==, "swordman")) {
        return &controller_swordman;
    }
    if (StrRel(name, ==, "spearman")) {
        return &controller_spearman;
    }
    if (StrRel(name, ==, "peasant")) {
        return &controller_peasant;
    }
    if (StrRel(name, ==, "default")) {
        return &controller_default;
    }
    
    fprintf(stderr, "(WARNING) get_controller: \'%s\' is not a valid controller name!\n", name);
    return &controller_default;
}



void no_action(GameObject_Controller *controller, GameObject *object) {
    return;
}

void default_on_tick(GameObject_Controller *controller, GameObject *object) {
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
        new_x += movement_x * -1;
    } else if (new_y <= map_min_y) {
        new_y += movement_y * -1;
    } else if (new_x >= map_max_x - object_height) {
        new_x -= object_height - movement_x * -1;
    } else if (new_y >= map_max_y - object_height) {
        new_y -= object_height - movement_y * -1;
    }
    //new_x = MIN(MAX(new_x, 0), max_x - object_height);
    //new_y = MIN(MAX(new_y, 0), max_y - object_height);

    /*boolean result = FALSE;*/
    object->m->move_to(object, new_x, new_y, FALSE);
    object->m->on_tick(object);
}

boolean default_shoot(GameObject_Controller *controller, GameObject *object) {
    (void)controller;
    (void)object;
    return TRUE;
}

boolean peasant_shoot(GameObject_Controller *controller, GameObject *object) {
    (void)controller;
    (void)object;
    return farm_square(object);
}

boolean farm_square(GameObject *object) {
    size_t x = object->m->get_x(object); 
    size_t y = object->m->get_y(object);
    Square *current_square = global_map->m->get_square(global_map, x, y);
    if (current_square == NULL) {
        GameLog_println("Nothing here...");
        return FALSE;
    }
    TerrainType *type = current_square->m->get_terrain_type(current_square);
    //fprintf(stderr, "Looking at mah '%s'!\n", type->m->get_name(type));
    
    switch (type->m->get_tag(type)) {
        case TERRAIN_STONE:
            GameLog_println("*You picked up some rocks*");
            break;
        case TERRAIN_GRASS:
            GameLog_println("Grass");
            break;
        case TERRAIN_WHEAT:
            GameLog_println("Wheat");
            break;
        case TERRAIN_WATER:
            GameLog_println("*You drank some water*");
            break;
        case TERRAIN_TREE:
            GameLog_println("Tree");
            break;
        default:
            GameLog_println("Get the hell off my lawn!");
    }

    return TRUE;
}
