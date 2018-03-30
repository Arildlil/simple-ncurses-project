#include "rendering.h"
#include "utils.h"
#include "curses.h"
#include "player.h"
#include "curses.h"

#include <stdlib.h>
#include <string.h>
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
        return FALSE;
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

        frames[i]->width = width;
        frames[i]->height = height;
        fprintf(stderr, "frames[%d]->width: %d, ->height: %d\n", i, frames[i]->width, frames[i]->height);

        for (j = 0; j < pixel_count; j++) {
            Pixel *current_pixel = &frames[i]->pixels[j];
            current_pixel->color = COLOR_PAIR_NONE;
            current_pixel->symbol = ' ';
        }
    }

    return TRUE;
}

void Rendering_convert_coordinates(Map *map, int half_screen_width, int half_screen_height, 
    int center_x, int center_y, RenderCoordinateBorders *borders) {
    
    int map_max_x = map->m->get_max_x(map);
    int map_max_y = map->m->get_max_y(map);

    //fprintf(stderr, "Convert: w %d, h %d, (%d, %d)\n", half_screen_width, half_screen_height, center_x, center_y);

    borders->left_x = center_x - half_screen_width;
    borders->right_x = center_x + half_screen_width;
    borders->top_y = center_y - half_screen_height;
    borders->bottom_y = center_y + half_screen_height;
    borders->index_left_x = borders->left_x + map_max_x;
    borders->index_right_x = borders->right_x + map_max_x;
    borders->index_top_y = borders->top_y + map_max_y;
    borders->index_bottom_y = borders->bottom_y + map_max_y;
}

void Rendering_add_background(Map *map, int center_x, int center_y) {
    assert(map);

    FrameBuffer *current_frame = frames[current_frame_index];
    size_t current_height = current_frame->height;
    size_t current_width = current_frame->width;

    assert(current_height != 0);
    assert(current_width != 0);

    RenderCoordinateBorders borders = {0};
    Rendering_convert_coordinates(map, current_width/2, current_height/2, center_x, center_y, &borders);
    
    /*fprintf(stderr, "top_y: %d, left_x: %d, bottom_y: %d, right_x: %d, current_height: %d, current_width: %d\n", 
        borders.top_y, borders.left_x, borders.bottom_y, borders.right_x, current_height, current_width);
    */

    int from_x, from_y; 
    size_t to_x, to_y, j, k;
    
    /* Reset the buffer */
    TerrainType *terrain = terrain_default;

    for (from_y = borders.top_y, to_y = 0; from_y < borders.bottom_y && to_y < current_height; from_y++, to_y++) {
        for (from_x = borders.left_x, to_x = 0; from_x < borders.right_x && to_x < current_width; from_x++, to_x++) {
            size_t index = to_y * current_width + to_x;
            Pixel *current_pixel = &current_frame->pixels[index];
            current_pixel->color = terrain->colors;
            current_pixel->symbol = terrain->image->pixels[0][0];
        }
    }

    /* Paint the terrain */
    for (from_y = borders.top_y, to_y = 0; from_y < borders.bottom_y && to_y < current_height; from_y++, to_y++) {
        //fprintf(stderr, "from_y:  %d, to_y: %d, borders.bottom_y: %d\t", from_y, to_y, borders.index_bottom_y);
        for (from_x = borders.left_x, to_x = 0; from_x < borders.right_x && to_x < current_width; from_x++, to_x++) {
            
            size_t index = to_y * current_width + to_x;
            Square *current_square = map->m->get_square(map, from_x, from_y);
            if (current_square == NULL) {
                continue;
            }

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
                    size_t inner_index = (to_y + j) * current_width + to_x + k;
                    Pixel *current_pixel = &current_frame->pixels[inner_index];
                    current_pixel->color = color;
                    current_pixel->symbol = cur_pixel;
                }
            }
        }
    }

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