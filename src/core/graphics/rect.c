#include "../../include/rect.h"

#include <stdlib.h>
#include <assert.h>


/*
 * Constructor for the Rect type.
 */
Rect* Rect_init(Rect *rect, int x, int y, int width, int height) {
    assert(rect != NULL);

    if (width < 0) width = 0;
    if (height < 0) height = 0;

    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;

    return rect;
}

/*
 * Move the coordinates of the Rect.
 */
Rect* Rect_move(Rect *rect, int x, int y) {
    assert(rect != NULL);

    rect->x += x;
    rect->y += y;

    return rect;
}

