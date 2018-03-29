#include "rendering.h"
#include "utils.h"
#include "curses.h"
#include "player.h"
#include "curses.h"

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

static void Rendering_convert_coordinates(Map *map, int screen_width, int screen_height, 
    int center_x, int center_y, int *left_x, int *top_y, size_t *index_left_x, size_t *index_top_y) {
    
    int map_max_x = map->m->get_max_x(map);
    int map_max_y = map->m->get_max_y(map);

    *left_x = center_x - screen_width/2;
    *top_y = center_y - screen_height/2;
    *index_left_x = *left_x + map_max_x;
    *index_top_y = *top_y + map_max_y;
}

void Rendering_add_background(Map *map, int center_x, int center_y) {
    assert(map);

    FrameBuffer *current_frame = frames[current_frame_index];
    size_t current_height = current_frame->height;
    size_t current_width = current_frame->width;

    int leftmost_x = 0;
    int topmost_y = 0;
    size_t index_leftmost_x = 0;
    size_t index_topmost_y = 0;
    Rendering_convert_coordinates(map, screen_width, screen_height, center_x, center_y,
        &leftmost_x, &topmost_y, &index_leftmost_x, &index_topmost_y);

    /* TODO: Make sure only the map around the center is rendered! */

    /*
    int x, y, j, k;
    for (y = 0; y < map->m->get_max_y(map); y++) {
        for (x = 0; x < map->m->get_max_x(map); x++) {
            Square *current_square = map->m->get_square(map, x, y);
            assert(current_square);

            TerrainType *terrain = current_square->m->get_terrain_type(current_square);
            if (terrain->m->get_tag(terrain) == TERRAIN_NONE) {
                continue;
            }

            Color_Pair color = terrain->m->get_colors(terrain);
            Image *image = terrain->m->get_image(terrain);
            char **pixels = image->get_pixels(image);

            for (j = 0; j < image->get_height(image); j++) {
                for (k = 0; k < image->get_width(image); k++) {
                    char cur_pixel = pixels[j][k];
                    //mvaddch(y+j, x+k, cur_pixel);
                    size_t index = y * current_width + x;
                    Pixel *current_pixel = &current_frame->pixels[index];
                    current_pixel->color = color;
                    current_pixel->symbol = cur_pixel;
                }
            }
        }
    }*/
} 

void Rendering_render_frame() {
    if (inited == FALSE) {
        return;
    }

    size_t x, y;
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