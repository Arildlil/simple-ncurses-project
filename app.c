#include "test_headers.h"
#include "curses.h"
#include "utils.h"
#include "unit_images.h"
#include "units.h"
#include "gameobject.h"
#include "units.h"
#include "player_controls.h"
#include "player.h"
#include "terrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <assert.h>
#include <time.h>
#include <sys/param.h>

#include <ncurses.h>



/* ----- | Prototypes | ------ */

static int init();
static void process_input();
static void on_tick(GameObject objects[], int num_elements);
static void Curses_redraw_objects(Map *map, GameObject objects[], int num_elements);
static void render_objects(Map *map, GameObject objects[], int num_objects);
static void render(Surface *surfaces[], int num_objects);
static void cleanup(int sig);

/* ----- | Static Variables | ------ */

#define FPS 10
#define US_PER_SEC 1000000
#define UPDATE_RATE_US (US_PER_SEC / FPS)
#define MOVE_RANGE 5
static int MIDDLE_X = 0;
static GameObject *hero;

/* ----- | Functions | ----- */



static void default_on_tick(GameObject_Controller *controller, GameObject *object) {
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

    if (new_x <= object_height) {
        new_x += object_height + movement_x * -1;
    } else if (new_y <= object_height) {
        new_y += object_height + movement_y * -1;
    } else if (new_x >= max_x - object_height) {
        new_x -= max_x - object_height - movement_x * -1;;
    } else if (new_y >= max_y - object_height) {
        new_y -= max_y - object_height - movement_y * -1;;
    }
    new_x = MIN(MAX(new_x, 0), max_x - object_height);
    new_y = MIN(MAX(new_y, 0), max_y - object_height);

    boolean result = FALSE;
    object->m->move_to(object, new_x, new_y, FALSE);
    object->m->on_tick(object);
}

static void generate_default_map(Map *map) {
    int map_width = map->m->get_width(map);
    int map_height = map->m->get_height(map);
    int w, h;
    /*
    for (h = 0; h < map_height; h++) {
        for (w = 0; w < map_width; w++) {
            Square_init(map, w, h, TERRAIN_GRASS);
        }
    }*/

    enum {NUM_TERRAIN_PIECES = 5};
    TerrainType_Tag tags[NUM_TERRAIN_PIECES] = {
        TERRAIN_TREE, TERRAIN_TREE, TERRAIN_STONE, TERRAIN_STONE, TERRAIN_TREE
    };
    int x_coords[NUM_TERRAIN_PIECES] = {15, 25, 50, 65, 90};
    int y_coords[NUM_TERRAIN_PIECES] = {4, 15, 7, 23, 6};

    int i;
    for (i = 0; i < NUM_TERRAIN_PIECES; i++) {
        Square_init(map, x_coords[i], y_coords[i], tags[i]);
    }
}

int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    init();

    (void)argc;
    (void)argv;

    Player player, neutrals;
    Player_init(&player, COLOR_PAIR_RED, TRUE);
    Player_init(&neutrals, COLOR_PAIR_YELLOW, FALSE);

    Map default_map;
    Map_init(&default_map, max_x, max_y);
    generate_default_map(&default_map);

    char *units_to_spawn[] = {
        "archer",
        "archer",
        "swordman",
        "spearman"
    };

    GameObject_Controller random_controller;
    GameObject_Controller_Methods random_methods = {.on_tick = default_on_tick};
    GameObject_Controller_init(&random_controller, &random_methods);
    //GameObject_Controller_init(&random_controller, NULL);

    MIDDLE_X = max_x / 2;
    #define NUM_TROOPS 4
    #define NUM_OBJECTS (NUM_TROOPS + 1)
    GameObject objects[NUM_OBJECTS];
    int i;
    for (i = 0; i < NUM_TROOPS; i++) {
        Units_init_name(&objects[i], &neutrals, MIDDLE_X, 10+5*i, units_to_spawn[i]);
        objects[i].m->set_controller(&objects[i], &random_controller);
    }
    /* Initialize a unit for the player to control */
    Units_init_archer(&objects[NUM_OBJECTS-1], &player, 20, 20);
    hero = &objects[NUM_OBJECTS-1];

    int counter = 0;
    while (1) {
        on_tick(objects, NUM_TROOPS);
        render_objects(&default_map, objects, NUM_OBJECTS);

        process_input();

        counter++;
        #ifdef DEBUG
        if (counter > 50)
            break;
        #endif
        usleep(UPDATE_RATE_US);
    }
    printf("max_x: %d, max_y: %d\n", max_x, max_y);
    
    cleanup(OK);
}

static void process_input() {
    char input = getch();

    switch (input) {
        case ERR: 
            break;
        case 'a':
            /* Intentional fall-through */
        case 'd':
            /* Intentional fall-through */
        case 'w':
            /* Intentional fall-through */
        case 's':
            /* Intentional fall-through */
        case 'q':
            /* Intentional fall-through */
        case 'e':
            /* Intentional fall-through */
        case '<':
            /* Intentional fall-through */
        case 'x':
            PlayerControls_handle_input_char(input, hero);
            break;
    }
}

/*
 * Go through the controller of all the units and perform their
 * 'on_tick' method.
 */
static void on_tick(GameObject objects[], int num_elements) {
    int i;
    for (i = 0; i < num_elements; i++) {
        GameObject *current_object = &objects[i];
        if (current_object == NULL) {
            continue;
        }
        GameObject_Controller *controller = current_object->m->get_controller(current_object);
        if (controller != NULL) {
            controller->m->on_tick(controller, current_object);
        }
    }
}

static void render_objects(Map *map, GameObject objects[], int num_objects) {
    assert(objects != NULL);
    assert(num_objects >= 0);

    Curses_redraw_objects(map, objects, num_objects);
}

/*
 * Redraws the GameObjects.
 */
static void Curses_redraw_objects(Map *map, GameObject objects[], int num_elements) {
    int i, j, k, x, y;
    getmaxyx(stdscr, max_y, max_x);
    clear();

    /* Draw the background Map. */
    for (y = 0; y < map->m->get_max_y(map); y++) {
        for (x = 0; x < map->m->get_max_x(map); x++) {
            Square *current_square = map->m->get_square(map, x, y);
            if (current_square == NULL) {
                fprintf(stderr, "Curses_redraw_objects: Warning - square (%d, %d) was NULL!\n", x, y);
                continue;
            }

            TerrainType *terrain = current_square->m->get_terrain_type(current_square);
            if (terrain->m->get_tag(terrain) == TERRAIN_NONE) {
                continue;
            }

            Color_Pair color = terrain->m->get_colors(terrain);
            Image *image = terrain->m->get_image(terrain);
            char **pixels = image->get_pixels(image);

            attron(COLOR_PAIR(color));
            for (j = 0; j < image->get_height(image); j++) {
                for (k = 0; k < image->get_width(image); k++) {
                    char cur_pixel = pixels[j][k];
                    mvaddch(y+j, x+k, cur_pixel);
                }
            }
            //mvaddch(i, j, ' ');
            attroff(COLOR_PAIR(color));
        }
    }

    /* Draw the GameObjects. */
    for (i = 0; i < num_elements; i++) {
        GameObject* cur = &objects[i];
        /*assert(cur != NULL);*/
        /*
        if (cur->state == DEAD) {
            continue;
        }*/
        if (cur == NULL) {
            continue;
        }
        
        char **pixels = cur->m->get_pixels(cur);
        assert(pixels != NULL);
        dprintf("Object <%d> (x,y)\n", i);
 
        Player *owner = cur->m->get_owner(cur);
        Color_Pair colors = owner->m->get_colors(owner);
        attron(COLOR_PAIR(colors));
        for (j = 0; j < cur->m->get_height(cur); j++) {
            for (k = 0; k < cur->m->get_width(cur); k++) {
                char cur_pixel = pixels[j][k];
                dprintf("\t(%d,", cur->x+k);
                dprintf("%d) ", cur->y+j);
                mvaddch(cur->y+j, cur->x+k,cur_pixel);
            }
            dnprintf("\n");
        }
        attroff(COLOR_PAIR(colors));
    }
    refresh();
}

/* 
 * Render the specified objects.
 */
static void render(Surface *surfaces[], int num_objects) {
    assert(surfaces != NULL);
    assert(num_objects >= 0);

    Curses_redraw(surfaces, num_objects);
}

/*
 * Initialization code.
 */
static int init() {
    UnitImages_init();
    srand(clock());

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGKILL, cleanup);
    Curses_init();
    return OK;
}

/*
 * Clean up before exiting.
 * 
 * @arg sig: Signal that caused this to be called. 
 *   0 on normal termination.
 */
static void cleanup(int sig) {
    (void)sig;
    Curses_exit();
    dprintf("Cleaning up and exiting with sig %d...\n", sig);
    exit(0);
}