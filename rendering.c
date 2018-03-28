#include "rendering.h"
#include "utils.h"
#include "curses.h"
#include "player.h"
#include "curses.h"
#include "terrain.h"

#include <ncurses.h>
#include <assert.h>



/* ----- | Type Declarations | ------ */

typedef struct Pixel {
    Color_Pair color;
    char symbol;
} Pixel;

typedef struct FrameBuffer {
    size_t width;
    size_t height;
    Pixel *pixels;
} FrameBuffer;



/* ----- | Static Variables | ------ */

enum {
    NUM_FRAME_BUFFERS = 2,
};

static boolean inited = FALSE;
static size_t screen_width = 0;
static size_t screen_height = 0;

size_t current_frame_index = 0;
FrameBuffer *frames[NUM_FRAME_BUFFERS];



/* ----- | Functions | ----- */

boolean Rendering_init(size_t width, size_t height) {
    if (inited == TRUE) {
        return;
    }

    inited = TRUE;
    screen_width = width;
    screen_height = height;
    current_frame_index = 0;
    size_t pixel_count = width * height;
    int i, j;
    for (i = 0; i < NUM_FRAME_BUFFERS; i++) {
        
        if ((frames[i] = calloc(1, sizeof(FrameBuffer))) == NULL) {
            return FALSE;
        }
        if ((frames[i]->pixels = calloc(pixel_count, sizeof(Pixel))) == NULL) {
            return FALSE;
        }

        for (j = 0; j < pixel_count; j++) {
            Pixel *current_pixel = &frames[i]->pixels[j];
            current_pixel->color = COLOR_PAIR_NONE;
            current_pixel->symbol = ' ';
        }
    }

    return TRUE;
}

void Rendering_render_frame() {
    if (inited == FALSE) {
        return;
    }

    int x, y;
    FrameBuffer *current_frame = frames[current_frame_index];
    size_t current_height = current_frame->height;
    size_t current_width = current_frame->width;
    for (y = 0; y < current_height; y++) {
        for (x = 0; x < current_width; x++) {
            size_t index = y * current_width + x;
            Pixel *current_pixel = &current_frame->pixels[index];
            attron(COLOR_PAIR(current_pixel->color));
            mvaddch(y, x, current_pixel->symbol);
            attroff(COLOR_PAIR(current_pixel->color));
        }
    }

    current_frame_index = ++current_frame_index % NUM_FRAME_BUFFERS;
}

void Rendering_render_object(GameObject *object) {
    if (inited == FALSE) {
        return;
    }

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