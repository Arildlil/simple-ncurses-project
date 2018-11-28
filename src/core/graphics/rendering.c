#include "../../include/rendering.h"
#include "rendering_internals.h"
#include "../../include/game_log.h"
#include "../../include/utils.h"
#include "../../include/curses.h"
#include "../../include/player.h"

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <assert.h>



/* ----- | Function Prototypes | ----- */

static void clear_framebuffer(FrameBuffer *frame_buffer, TerrainType *default_terrain);
static void paint_object(GameObject *object, int x, int y, FrameBuffer *frame_buffer,
    RenderCoordinateBorders *borders);
static void paint_image(Image *image, int x, int y, Color_Pair color, FrameBuffer *frame_buffer,
    RenderCoordinateBorders *borders);
static void paint_terrain(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map);
static void paint_objects(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map, 
    GameObject *objects[], int num_elements);
static void paint_bottom_menu(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, 
    Map *map, int start_x, int end_x, int start_y, int end_y);



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

    size_t i, j;
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
        /*
        fprintf(stderr, "frames[%d]->width: %d, ->height: %d\n", 
            i, frames[i]->width, frames[i]->height);
        */

        for (j = 0; j < pixel_count; j++) {
            Pixel *current_pixel = &frames[i]->pixels[j];
            current_pixel->color = COLOR_PAIR_NONE;
            current_pixel->symbol = ' ';
        }
    }

    return TRUE;
}

FrameBuffer *get_frame_buffer() {
    return frames[current_frame_index];
}

int Rendering_get_screen_height() {
    return screen_height;
}

int Rendering_get_screen_width() {
    return screen_width;
}

int Rendering_get_menu_bottom_height() {
    return menu_bottom_height;
}

int Rendering_get_menu_bottom_width() {
    return menu_bottom_width;
}

void Rendering_convert_coordinates(Map *map, int half_screen_width, int half_screen_height, 
    int center_x, int center_y, RenderCoordinateBorders *borders) {
    
    (void)map;
    /*
    int map_max_x = map->m->get_max_x(map);
    int map_max_y = map->m->get_max_y(map);
    */

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
 * Calculates a 1D index into a frame buffer from 2D coordinates.
 * 
 * @arg fb_x: The x index in the frame buffer.
 * @arg fb_y: The y index in the frame buffer.
 * @arg width: The width of a line in the frame buffer.
 * @return: A 1D index on success, -1 otherwise.
 */
static inline int inner_i(int fb_x, int fb_y, int width) {
    if (fb_x < 0 || fb_y < 0) return -1;
    return (fb_y * width + fb_x);
}

static void paint_object(GameObject *object, int x, int y, FrameBuffer *frame_buffer,
    RenderCoordinateBorders *borders) {

    Player *owner = object->m->get_owner(object);
    Color_Pair colors = owner->m->get_colors(owner);
    Image *image = object->m->get_image(object);

    paint_image(image, x, y, colors, frame_buffer, borders);
}

static void paint_image(Image *image, int x, int y, Color_Pair color, FrameBuffer *frame_buffer,
    RenderCoordinateBorders *borders) {

    size_t current_height = borders->bottom_y - borders->top_y + 1;
    size_t current_width = borders->right_x - borders->left_x + 1;

    char **pixels = image->get_pixels(image);
    int image_height = image->get_height(image);
    int image_width = image->get_width(image);
    int half_image_height = image_height / 2;
    int half_image_width = image_width / 2;
    
    int i, j;
    attron(COLOR_PAIR(color));
    for (i = 0; i < image_height && y - borders->top_y + i < (int)current_height; i++) {
        for (j = 0; j < image_width && x - borders->left_x + j < (int)current_width; j++) {
            char current_pixel_value = pixels[i][j];
            int buffer_y_target = y - borders->top_y + i - half_image_height;
            int buffer_x_target = x - borders->left_x + j - half_image_width;
            int inner_index = inner_i(buffer_x_target, buffer_y_target, current_width);
            if (inner_index < 0) break;
            if (inner_index >= frame_buffer->max_index) break;
            Pixel *current_pixel = &frame_buffer->pixels[inner_index];
            current_pixel->color = color;
            current_pixel->symbol = current_pixel_value;
        }
    }
    attroff(COLOR_PAIR(color));
}

/*
 * Paint the terrain into the frame buffer.
 */
static void paint_terrain(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map) {
    int from_x, from_y; 
    size_t to_x, to_y;

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
            paint_image(image, from_x, from_y, color, frame_buffer, borders);
        }
    }
}

/*
 * Paint the visible GameObjects into the frame buffer.
 */
static void paint_objects(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, Map *map, 
    GameObject *objects[], int num_elements) {
    
    (void)map;

    int i;
    for (i = 0; i < num_elements; i++) {
        GameObject *object = objects[i];
        int object_x = object->m->get_x(object);
        int object_y = object->m->get_y(object);
        if (object_x < borders->left_x || object_x > borders->right_x || 
            object_y < borders->top_y || object_y > borders->bottom_y ) {
            
            continue;
        }

        paint_object(object, object_x, object_y, frame_buffer, borders);
    }
}

void render_char(int fb_x, int fb_y, char pixel, Color_Pair color,
    FrameBuffer *frame_buffer, int current_width) {

    if (fb_x < 0 || fb_y < 0) return;
    if (fb_x >= current_width) return;
    assert(frame_buffer);

    attron(COLOR_PAIR(color));
    int inner_index = inner_i(fb_x, fb_y, current_width);
    assert(inner_index >= 0);
        
    Pixel *current_pixel = &frame_buffer->pixels[inner_index];
    current_pixel->color = color;
    current_pixel->symbol = pixel;
    attroff(COLOR_PAIR(color));
}

void render_line_char(int fb_x, int fb_y, char pixel, int repeats, Color_Pair color,
    FrameBuffer *frame_buffer, int current_width) {
    
    assert(repeats >= 0);
    if (repeats == 0) return;

    char pixels[repeats];
    memset(pixels, pixel, repeats);
    render_line(fb_x, fb_y, pixels, repeats, color, frame_buffer, current_width);
}

void render_line(int fb_x, int fb_y, char *pixels, int width, Color_Pair color,
    FrameBuffer *frame_buffer, int current_width) {
    
    if (fb_x < 0 || fb_y < 0) return;
    assert(pixels);
    assert(width >= 0);
    assert(frame_buffer);

    attron(COLOR_PAIR(color));
    int i, to_x;
    for (i = 0, to_x = fb_x; i < width && to_x < current_width; i++, to_x++) {
        int inner_index = inner_i(to_x, fb_y, current_width);
        assert(inner_index >= 0);
        
        Pixel *current_pixel = &frame_buffer->pixels[inner_index];
        current_pixel->color = color;
        current_pixel->symbol = pixels[i];
    }
    attroff(COLOR_PAIR(color));
}

void render_line_vert_char(int fb_x, int fb_y, char pixel, int repeats, 
    Color_Pair color, FrameBuffer *frame_buffer, int current_width) {

    int j;
    for (j = 0; j < repeats; j++) {
        render_char(fb_x, fb_y + j, pixel, color, frame_buffer, current_width);
    }
}

void render_line_vert(int fb_x, int fb_y, char *pixels, int width, 
    Color_Pair color, FrameBuffer *frame_buffer, int current_width) {

    int j;
    for (j = 0; j < width; j++) {
        render_char(fb_x, fb_y + j, pixels[j], color, frame_buffer, current_width);
    }
}

/*
 * Paint the bottom menu.
 */
static void paint_bottom_menu(FrameBuffer *frame_buffer, RenderCoordinateBorders *borders, 
    Map *map, int start_x, int end_x, int start_y, int end_y) {
    
    (void)map;
    (void)borders;
    assert(frame_buffer);

    size_t current_height = end_y - start_y;
    size_t current_width = end_x - start_x;

    char menu_border_pixel = '=';
    Color_Pair menu_border_color = COLOR_PAIR_WHEAT;
    char menu_vert_border_pixel = '|';
    Color_Pair menu_vert_border_color = COLOR_PAIR_WHEAT;

    /* Render top border line */
    render_line_char(start_x, start_y, menu_border_pixel, current_width, 
        menu_border_color, frame_buffer, current_width);
    
    /* Render right and left vertical border lines */
    render_line_vert_char(start_x, start_y, menu_vert_border_pixel, current_height,
        menu_vert_border_color, frame_buffer, current_width);

    render_line_vert_char(end_x-1, start_y, menu_vert_border_pixel, current_height,
        menu_vert_border_color, frame_buffer, current_width);
}

void Rendering_fill_framebuffer(Map *map, int center_x, int center_y, GameObject *objects[], int num_elements) {
    assert(map);

    FrameBuffer *current_frame = frames[current_frame_index];
    size_t current_height = current_frame->height;
    size_t current_width = current_frame->width;

    size_t menu_bottom_top_y = current_height - menu_bottom_height;
    size_t menu_bottom_bot_y = current_height;
    
    fprintf(stderr, "curH: %d, curW: %d, topY: %d, botY: %d\n", 
        (int)current_height, (int)current_width, (int)menu_bottom_top_y, (int)menu_bottom_bot_y);

    /* Don't draw under the bottom menu */
    current_height -= menu_bottom_height;
    center_y += menu_bottom_height / 2;

    /* Used for the terrain part of the window */
    RenderCoordinateBorders borders = {0};
    Rendering_convert_coordinates(map, (int)current_width / 2, (int)current_height / 2, (int)center_x, (int)center_y, 
        &borders);

    fprintf(stderr, "(borders) bottom_y: %d, left_x: %d\t", borders.bottom_y, borders.left_x);

    /* Used for the bottom menu part of the window */
    RenderCoordinateBorders borders_menu_bottom = {0};
    Rendering_convert_coordinates(map, menu_bottom_width / 2, menu_bottom_height / 2, center_x, 
        borders.bottom_y + menu_bottom_height / 2, &borders_menu_bottom);

    fprintf(stderr, "(MENUborders) top_y: %d, bottom_y: %d, right_x: %d, left_x: %d\n", borders_menu_bottom.top_y, 
        borders_menu_bottom.bottom_y, borders_menu_bottom.right_x, borders_menu_bottom.left_x);
    
    /* Clear and then fill the frame buffer */
    clear_framebuffer(current_frame, terrain_default);
    paint_terrain(current_frame, &borders, map);
    paint_objects(current_frame, &borders, map, objects, num_elements);
    paint_bottom_menu(current_frame, &borders_menu_bottom, map, 
        0, menu_bottom_width, menu_bottom_top_y, menu_bottom_bot_y);

    /* Draw the GameLog */
    GameLog_draw();
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