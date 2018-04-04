#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "curses.h"
#include "utils.h"
#include "gameobject.h"
#include "player.h"



/* ----- | Typedefs and Forward Declarations | ----- */

/* GameObject */
typedef struct GameObject GameObject;
typedef struct GameObject_Methods GameObject_Methods;
typedef struct GameObject_Controller GameObject_Controller;
typedef struct GameObject_Controller_Methods GameObject_Controller_Methods;

/* Orders */
typedef struct Destination Destination;
typedef struct Order Order;





/* ----- | Constants | ----- */

enum {
    MAX_ORDERS = 10
};

typedef enum Direction {
    NORTH, SOUTH, EAST, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST
} Direction;



/* ----- | Type Definitions | ----- */

/* Orders */

typedef enum Order_Type {
    ORDER_TYPE_NONE, ORDER_TYPE_MOVE, ORDER_TYPE_ATTACK
} Order_Type;

typedef enum Destination_Type {
    DEST_TYPE_GAMEOBJECT, DEST_TYPE_COORDINATES
} Destination_Type; 

struct Destination {
    Destination_Type desttype;
    union {
        GameObject *object;
        Coordinates coordinates;
    };
};

struct Order {
    boolean is_active;
    Order_Type type;
    Destination destination;
};



/* GameObjects */

struct GameObject {
    Player *owner;
    int x;
    int y;
    Surface surface;
    boolean active;
    Order order_queue[MAX_ORDERS];
    int order_count;
    int current_order_index;
    Direction direction;
    size_t memory_id;

    /* Controls the behavior of this object, if set */
    GameObject_Controller *controller;

    /* Common methods for all GameObjects */
    GameObject_Methods *m;
};

struct GameObject_Controller {
    /* Contains function pointers. These can vary from 
     * object to object. */
    GameObject_Controller_Methods *m;
};

struct GameObject_Methods {
    /* Utils */
    void (*free)(struct GameObject *object);
    boolean (*is_active)(struct GameObject *object);
    Player *(*get_owner)(struct GameObject *object);
    GameObject_Controller *(*get_controller)(struct GameObject *object);
    void (*set_controller)(struct GameObject *object, struct GameObject_Controller *controller);
    Direction (*get_direction)(struct GameObject *object);
    void (*set_direction)(struct GameObject *object, Direction direction);

    /* Orders */
    void (*on_tick)(struct GameObject *object);
    int (*get_order_count)(struct GameObject *object);
    Order *(*get_current_order)(struct GameObject *object);
    boolean (*move_to)(struct GameObject *object, int x, int y, boolean queued);
    boolean (*attack)(struct GameObject *object, struct GameObject *target, boolean queued);
    boolean (*shoot)(struct GameObject *object);

    /* Movement and coordinates */
    void (*movement)(struct GameObject *object, int x, int y);
    int (*get_x)(struct GameObject *object);
    int (*get_y)(struct GameObject *object);
    void (*set_x)(struct GameObject *object, int x);
    void (*set_y)(struct GameObject *object, int y);
    void (*set_xy)(struct GameObject *object, int x, int y);
    Surface *(*get_surface)(struct GameObject *object);

    /* Image */
    Image *(*get_image)(struct GameObject *object);
    int (*get_width)(struct GameObject *object);
    int (*get_height)(struct GameObject *object);
    char **(*get_pixels)(struct GameObject *object);
};

/* This struct should be filled manually! */
struct GameObject_Controller_Methods {
    void (*on_tick)(struct GameObject_Controller *controller, struct GameObject *object);
    boolean (*shoot)(struct GameObject_Controller *controller, struct GameObject *object);
};



/* ----- | Constructors | ----- */

/*
 * Constructor for GameObjects.
 * 
 * @arg object: The object to initialize.
 * @arg x: The x-coordinate to spawn at.
 * @arg y: The y-coordinate to spawn at.
 * @arg image: The image to use. If this is NULL, the GameObject will NOT be rendered!
 * @arg options (Optional): The surface options to use. Uses default options if NULL.
 * @return: The object argument.
 */
GameObject* GameObject_init(GameObject *object, Player *owner, int x, int y, const Image *image,
    Surface_Options *options);

/*
 * Constructor for GameObject_Controllers. One controller can be shared by several GameObjects.
 * 
 * @arg controller: The controller object to initialize.
 * @arg methods: A pointer to a struct with function pointers, working as methods for the object.
 * @return: The controller argument.
 */
GameObject_Controller* GameObject_Controller_init(GameObject_Controller *controller, 
    struct GameObject_Controller_Methods *methods);

/*
 * Clears up the Order struct for future use.
 * 
 * @arg order: The order struct to modify.
 */
void Orders_free(Order *order);

/*
 * Continue execution of the current Order of the object.
 * 
 * @arg object: The object that performs the Order.
 * @return: TRUE if Order is finished, FALSE otherwise.
 */
boolean Orders_update(GameObject *object);

/*
 * Issues a movement order to the unit.
 * 
 * @arg order: The order struct to modify.
 * @arg x: The target x-coordinate.
 * @arg y: The target y-coordinate.
 * @return: The order object.
 */
Order *Orders_move(Order *order, int x, int y);

/*
 * Issues an attack order on the target.
 * 
 * @arg order: The order struct to modify.
 * @arg target: The object to move towards for combat.
 * @return: The order object.
 */
Order *Orders_attack(Order *order, GameObject *target);

#endif