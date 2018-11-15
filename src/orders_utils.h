#ifndef ORDERS_UTILS_H
#define ORDERS_UTILS_H

#include "include/utils.h"
#include "include/gameobject.h"

void clear_order_queue(struct GameObject *object);
boolean insert_order(struct GameObject *object, Order *order, boolean queued);
void remove_current_order(struct GameObject *object);

#endif