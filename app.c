#include "test_headers.h"
#include "curses.h"
#include "utils.h"
#include "unit_images.h"
#include "units.h"
#include "gameobject.h"
#include "units.h"
#include "player_controls.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <time.h>

#include <ncurses.h>



/* ----- | Prototypes | ------ */

static int init();
static void process_input();
static void render_objects(GameObject objects[], int num_objects);
static void render(Surface *surfaces[], int num_objects);
static void cleanup(int sig);

/* ----- | Static Variables | ------ */

#define FPS 10
#define US_PER_SEC 1000000
#define UPDATE_RATE_US (US_PER_SEC / FPS)
static GameObject *hero;

/* ----- | Functions | ----- */



static void default_on_tick(GameObject_Controller *controller, GameObject *object) {
    int move_x = (rand() % 3) - 1;
    int move_y = (rand() % 3) - 1;
    
    object->m->movement(object, move_x, move_y);
}

int main(int argc, char *argv[]) {
    dnprintf("Main!\n");
    init();

    (void)argc;
    (void)argv;

    Player player, neutrals;
    Player_init(&player, COLOR_PAIR_RED, TRUE);
    Player_init(&neutrals, COLOR_PAIR_GREEN, FALSE);

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

    #define NUM_TROOPS 4
    #define NUM_OBJECTS (NUM_TROOPS + 1)
    GameObject objects[NUM_OBJECTS];
    int i;
    for (i = 0; i < NUM_TROOPS; i++) {
        Units_init_name(&objects[i], &neutrals, 0, 10+5*i, units_to_spawn[i]);
        objects[i].m->set_controller(&objects[i], &random_controller);
    }
    /* Initialize a unit for the player to control */
    Units_init_archer(&objects[NUM_OBJECTS-1], &player, 20, 20);
    hero = &objects[NUM_OBJECTS-1];

    int counter = 0;
    while (1) {
        render_objects(objects, NUM_OBJECTS);
        for (i = 0; i < NUM_TROOPS; i++) {
            /*objects[i].m->movement(&objects[i], 1, 0);*/
            
            random_controller.m->on_tick(&random_controller, &objects[i]);
        }

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
            PlayerControls_handle_input_char(input, hero);
            break;
    }
}

/*
 * Redraws the GameObjects.
 */
void Curses_redraw_objects(GameObject objects[], int num_elements) {
    int i, j, k;
    getmaxyx(stdscr, max_y, max_x);
    clear();
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

static void render_objects(GameObject objects[], int num_objects) {
    assert(objects != NULL);
    assert(num_objects >= 0);

    Curses_redraw_objects(objects, num_objects);
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