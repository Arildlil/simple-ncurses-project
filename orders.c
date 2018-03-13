#include "utils.h"
#include "gameobject.h"

void Orders_free(Order *order) {
    order->is_active = FALSE;
    order->type = ORDER_TYPE_NONE;
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