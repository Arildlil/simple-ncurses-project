#include "../../include/rendering.h"
#include "../../include/utils.h"
#include "../../include/curses.h"
#include "../../include/player.h"

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
    int max_index;
    Pixel *pixels;
} FrameBuffer;



/* ----- | Function Prototypes | ----- */

static void clear_framebuffer(FrameBuffer *frame_buffer, TerrainType *default_terrain);
static void paint_terrain(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map);
static void paint_objects(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map, 
    GameObject *objects[], int num_elements);
static void paint_bottom_menu(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map);



/* ----- | Static Variables | ------ */

enum {
    NUM_FRAME_BUFFERS = 1,
    MENU_BOTTOM_HEIGHT = 8,
};

static boolean inited = FALSE;
static size_t screen_width = 0;
static size_t screen_height = 0;
static size_t menu_bottom_width = 0;
static size_t menu_bottom_height = 0;

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

    menu_bottom_width = screen_width;
    menu_bottom_height = MENU_BOTTOM_HEIGHT;

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
        frames[i]->max_index = width * height - 1;
        fprintf(stderr, "frames[%d]->width: %d, ->height: %d\n", 
            i, frames[i]->width, frames[i]->height);

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

    borders->left_x = center_x - half_screen_width;
    borders->right_x = center_x + half_screen_width;
    borders->top_y = center_y - half_screen_height;
    borders->bottom_y = center_y + half_screen_height;
    borders->index_left_x = 0;
    borders->index_right_x = borders->right_x - borders->left_x;
    borders->index_top_y = 0;
    borders->index_bottom_y = borders->bottom_y - borders->top_y;
}

/*
 * Clear the framebuffer by filling it with the default terrain type.
 */
static void clear_framebuffer(FrameBuffer *frame_buffer, TerrainType *default_terrain) {
    assert(frame_buffer);
    assert(default_terrain);
    
    size_t current_height = frame_buffer->height;
    size_t current_width = frame_buffer->width;

    size_t to_x, to_y;
    for (to_y = 0; to_y < current_height; to_y++) {
        for (to_x = 0; to_x < current_width; to_x++) {
            size_t index = to_y * current_width + to_x;
            Pixel *current_pixel = &frame_buffer->pixels[index];
            current_pixel->color = default_terrain->colors;
            current_pixel->symbol = default_terrain->image->pixels[0][0];
        }
    }
}

/*
 * Paint the terrain into the frame buffer.
 */
static void paint_terrain(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map) {
    int from_x, from_y; 
    size_t to_x, to_y, j, k;

    size_t current_height = borders->bottom_y - borders->top_y + 1;
    size_t current_width = borders->right_x - borders->left_x + 1;
    
    for (from_y = borders->top_y, to_y = 0; from_y < borders->bottom_y && to_y < current_height; from_y++, to_y++) {
        for (from_x = borders->left_x, to_x = 0; from_x < borders->right_x && to_x < current_width; from_x++, to_x++) {
            
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

            for (j = 0; j < (size_t)image->get_height(image) && to_y + j < current_height; j++) {
                for (k = 0; k < (size_t)image->get_width(image) && to_x + k < current_width; k++) {
                    char cur_pixel = pixels[j][k];
                    int inner_index = (to_y + j) * current_width + to_x + k;
                    assert(inner_index >= 0);
                    if (inner_index >= frame_buffer->max_index) {
                        break;
                    }
                    Pixel *current_pixel = &frame_buffer->pixels[inner_index];
                    current_pixel->color = color;
                    current_pixel->symbol = cur_pixel;
                }
            }
        }
    }
}

/*
 * Paint the visible GameObjects into the frame buffer.
 */
static void paint_objects(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map, 
    GameObject *objects[], int num_elements) {
    
    (void)map;

    size_t j, k;

    size_t current_height = borders->bottom_y - borders->top_y + 1;
    size_t current_width = borders->right_x - borders->left_x + 1;

    int i;
    for (i = 0; i < num_elements; i++) {
        GameObject *object = objects[i];
        int object_x = object->m->get_x(object);
        int object_y = object->m->get_y(object);
        if (object_x < borders->left_x || object_x > borders->right_x || 
            object_y < borders->top_y || object_y > borders->bottom_y ) {
            
            continue;
        }

        char **pixels = object->m->get_pixels(object);
        Player *owner = object->m->get_owner(object);
        Color_Pair colors = owner->m->get_colors(owner);
        
        attron(COLOR_PAIR(colors));
        for (j = 0; j < (size_t)object->m->get_height(object) && object_y - borders->top_y + j < current_height; j++) {
            for (k = 0; k < (size_t)object->m->get_width(object) && object_x - borders->left_x + k < current_width; k++) {
                char cur_pixel = pixels[j][k];
                int inner_index = (object_y - borders->top_y + j) * 
                    current_width + object_x - borders->left_x + k;
                assert(inner_index >= 0);
                if (inner_index >= frame_buffer->max_index) {
                    break;
                }
                Pixel *current_pixel = &frame_buffer->pixels[inner_index];
                current_pixel->color = colors;
                current_pixel->symbol = cur_pixel;
            }
        }
        attroff(COLOR_PAIR(colors));
    }
}

/*
 * Paint the bottom menu.
 */
static void paint_bottom_menu(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map) {
    (void)map;

    size_t to_x, to_y, i, j;

    size_t current_height = borders->bottom_y - borders->top_y + 1;
    size_t current_width = borders->right_x - borders->left_x + 1;

    size_t first_inner_index = borders->top_y;
    fprintf(stderr, "top_y: %d, bottom_y: %d\n", borders->index_top_y, borders->index_bottom_y);

    /* MÅ FINNE RIKTIGE KOORDINATER */
    /*
    for (i = borders->top_y; i < borders->bottom_y; i++) {
        for (j = borders->left_x; j < borders->right_x; j++) {
            char cur_pixel = pixels[i][j];
            int inner_index = (object_y - borders->top_y + j) * 
                current_width + object_x - borders->left_x + k;
                assert(inner_index >= 0);
                if (inner_index >= frame_buffer->max_index) {
                    break;
                }
        }
    }*/

    /*
    for (from_y = borders->top_y, to_y = 0; from_y < borders->bottom_y && to_y < current_height; from_y++, to_y++) {
        for (from_x = borders->left_x, to_x = 0; from_x < borders->right_x && to_x < current_width; from_x++, to_x++) {
            
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

            for (j = 0; j < (size_t)image->get_height(image) && to_y + j < current_height; j++) {
                for (k = 0; k < (size_t)image->get_width(image) && to_x + k < current_width; k++) {
                    char cur_pixel = pixels[j][k];
                    int inner_index = (to_y + j) * current_width + to_x + k;
                    assert(inner_index >= 0);
                    if (inner_index >= frame_buffer->max_index) {
                        break;
                    }
                    Pixel *current_pixel = &frame_buffer->pixels[inner_index];
                    current_pixel->color = color;
                    current_pixel->symbol = cur_pixel;
                }
            }
        }
    }*/
}

void Rendering_fill_framebuffer(Map *map, int center_x, int center_y, GameObject *objects[], int num_elements) {
    assert(map);

    FrameBuffer *current_frame = frames[current_frame_index];
    size_t current_height = current_frame->height;
    size_t current_width = current_frame->width;

    /* Don't draw under the bottom menu */
    current_height -= menu_bottom_height;
    center_y += menu_bottom_height / 2;

    /* Used for the terrain part of the window */
    RenderCoordinateBorders borders = {0};
    Rendering_convert_coordinates(map, current_width / 2, current_height / 2, center_x, center_y, 
        &borders);

    fprintf(stderr, "(borders) bottom_y: %d, left_x: %d\n", borders.bottom_y, borders.left_x);

    /* Used for the bottom menu part of the window */
    RenderCoordinateBorders borders_menu_bottom = {0};
    Rendering_convert_coordinates(map, menu_bottom_width / 2, menu_bottom_height / 2, center_x, 
        borders.bottom_y + menu_bottom_height / 2, &borders_menu_bottom);
    
    /* Clear and then fill the frame buffer */
    clear_framebuffer(current_frame, terrain_default);
    paint_terrain(current_frame, &borders, map);
    paint_objects(current_frame, &borders, map, objects, num_elements);
    paint_bottom_menu(current_frame, &borders_menu_bottom, map);
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