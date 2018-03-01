#include "curses.h"




static const Surface_options default_options = {
    .bounce = FALSE,
};



void Surface_init(Surface *surface, int x, int y, char *content, 
    Surface_options* options) {
    
    surface->x = x;
    surface->y = y;
    surface->content = content;

    if (options) {
        surface->bounce = options->bounce;
    } else {
        surface->bounce = default_options.bounce;
    }
}

void Surface_move(Surface *surface, int x, int y) {
    surface->x += x;
    surface->y += y;

    if (surface->bounce == TRUE) {
        if (surface->x > max_x-1) {
            surface->x = max_x-1;
        }
        if (surface->y > max_y-1) {
            surface->y = max_y-1;
        }
    }
}