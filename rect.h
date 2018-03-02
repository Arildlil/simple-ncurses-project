#ifndef RECT_H
#define RECT_H

#include "utils.h"

typedef struct Rect {
    int x;
    int y;
    int left;
    int top;
    int width;
    int height;
} Rect;

/*
 * Constructor for the Rect type.
 * 
 * @arg rect: The Rect object to initialize.
 * @arg x: The x coordinate of the bottom, left corner of the Rect.
 * @arg y: The y coordinate of the bottom, left corner of the Rect.
 * @arg width: The width/length of the Rect.
 * @arg height: The height of the Rect.
 */
void Rect_init(Rect *rect, int x, int y, int width, int height);

/*
 * Move the coordinates of the Rect.
 * 
 * @arg rect: The Rect to move.
 * @arg x: The amount to move the Rect on the x-axis.
 * @arg y: The amount to move the Rect on the y-axis.
 */
void Rect_move(Rect *rect, int x, int y);

#endif