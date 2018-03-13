#include "utils.h"
#include "gameobject.h"



static int move_towards_goal(GameObject *object, int destination_x, int destination_y) {
    int cur_x = object->m->get_x(object);
    int cur_y = object->m->get_y(object);
    int move_x = 0;
    int move_y = 0;
    boolean x_reached = FALSE;
    boolean y_reached = FALSE;

    if (cur_x == destination_x) {
        x_reached = TRUE;
    } else if (cur_x < destination_x) {
        move_x++;
    } else {
        move_x--;
    }

    if (cur_y == destination_y) {
        y_reached = TRUE;
    } else if (cur_y < destination_y) {
        move_y++;
    } else {
        move_y--;
    }

    if (x_reached && y_reached) {
        return TRUE;
    }
    return FALSE;
}

static boolean update_move_order(Order *order, GameObject *object) {
    int destination_x = order->destination.coordinates.x;
    int destination_y = order->destination.coordinates.y;
    return move_towards_goal(object, destination_x, destination_y);
}

static boolean update_attack_order(Order *order, GameObject *object) {
    int destination_x = order->destination.coordinates.x;
    int destination_y = order->destination.coordinates.y;
    boolean target_reached = move_towards_goal(object, destination_x, destination_y);
    if (target_reached == TRUE) {
        return TRUE;
    }
    return FALSE;
}



void Orders_free(Order *order) {
    order->is_active = FALSE;
    order->type = ORDER_TYPE_NONE;
}

boolean Orders_update(Order *order, GameObject *object) {
    if (order->is_active == FALSE) {
        return FALSE;
    }

    switch (order->type) {
        case ORDER_TYPE_NONE:
            return FALSE;
        
        case ORDER_TYPE_MOVE:
            return update_move_order(order, object);

        case ORDER_TYPE_ATTACK:
            return update_attack_order(order, object);
            
        default:
            return FALSE;
    }
}

Order *Orders_move(Order *order, int x, int y) {
    order->is_active = TRUE;
    order->type = ORDER_TYPE_MOVE;
    order->destination.desttype = DEST_TYPE_COORDINATES;

    Coordinates *coordinates = &order->destination.coordinates;
    coordinates->x = x;
    coordinates->y = y;

    return order;
}

Order *Orders_attack(Order *order, GameObject *target) {
    order->is_active = TRUE;
    order->type = ORDER_TYPE_ATTACK;
    order->destination.desttype = DEST_TYPE_GAMEOBJECT;
    order->destination.object = target;

    return order;
}