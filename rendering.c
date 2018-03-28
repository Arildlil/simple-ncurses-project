#include "rendering.h"
#include "curses.h"
#include "player.h"

#include <ncurses.h>
#include <assert.h>

void Rendering_render_object(GameObject *object) {
    char **pixels = object->m->get_pixels(object);
    assert(pixels != NULL);
    dprintf("Object <%d> (x,y)\n", i);
 
    Player *owner = object->m->get_owner(object);
    Color_Pair colors = owner->m->get_colors(owner);
    attron(COLOR_PAIR(colors));
    int j, k;
    for (j = 0; j < object->m->get_height(object); j++) {
        for (k = 0; k < object->m->get_width(object); k++) {
            char object_pixel = pixels[j][k];
            dprintf("\t(%d,", object->x+k);
            dprintf("%d) ", object->y+j);
            mvaddch(object->y+j, object->x+k,object_pixel);
        }
        dnprintf("\n");
    }
    attroff(COLOR_PAIR(colors));
}