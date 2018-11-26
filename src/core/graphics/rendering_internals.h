#ifndef RENDERING_INTERNALS_H
#define RENDERING_INTERNALS_H

#include "../../include/colors.h"

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

/*
 * Returns a pointer to the main frame buffer.
 */
FrameBuffer *get_frame_buffer();

/*
 * Returns the current height of the screen.
 */
int Rendering_get_screen_height();

/*
 * Returns the current width of the screen.
 */
int Rendering_get_screen_width();

/*
 * Returns the current height of the bottom menu.
 */
int Rendering_get_menu_bottom_height();

/*
 * Returns the current width of the bottom menu.
 */
int Rendering_get_menu_bottom_width();

/*
 * Renders a single character in the frame buffer at the specified 2D coordinates.
 * 
 * @arg fb_x: The x index in the frame buffer.
 * @arg fb_y: The y index in the frame buffer.
 * @arg pixel: The pixel to draw.
 * @arg color: The color of the character to be rendered.
 * @arg frame_buffer: The frame buffer to draw to.
 * @arg current_width: The current width of a line in the frame buffer.
 */
void render_char(int fb_x, int fb_y, char pixel, Color_Pair color,
    FrameBuffer *frame_buffer, int current_width);

/*
 * Wrapper for 'render_line', where one specific character will be repeated 
 * several times.
 * 
 * @arg pixel: The pixel/character to repeat.
 * @ag repeats: The number of times the pixel/character will be repeated.
 * 
 * See 'render_line' for more argument info.
 */
void render_line_char(int fb_x, int fb_y, char pixel, int repeats, Color_Pair color,
    FrameBuffer *frame_buffer, int current_width);

/*
 * Renders a line in the frame buffer at the specified 2D coordinates.
 * All pixels in the pixel array will be drawn in the same color.
 * 
 * @arg fb_x: The x index in the frame buffer.
 * @arg fb_y: The y index in the frame buffer.
 * @arg pixels: The pixels (chars) on the line to be drawn.
 * @arg width: The length of the line to be rendered.
 * @arg color: The color of all the characters to be rendered.
 * @arg frame_buffer: The frame buffer to draw to.
 * @arg current_width: The current width of a line in the frame buffer.
 */
void render_line(int fb_x, int fb_y, char *pixels, int width, Color_Pair color,
    FrameBuffer *frame_buffer, int current_width);

/*
 * Renders a vertical line in the frame buffer at the specified 2D coordinates.
 * All pixels in the pixel array will be drawn in the same color and with the
 * same pixel value.
 * 
 * @arg fb_x: The x index in the frame buffer.
 * @arg fb_y: The y index in the frame buffer.
 * @arg pixels: The pixel on the line to be repeatedly drawn.
 * @arg width: The length of the line to be rendered.
 * @arg color: The color of all the characters to be rendered.
 * @arg frame_buffer: The frame buffer to draw to.
 * @arg current_width: The current width of a line in the frame buffer.
 */
void render_line_vert_char(int fb_x, int fb_y, char pixel, int repeats, 
    Color_Pair color, FrameBuffer *frame_buffer, int current_width);

/*
 * Renders a vertical line in the frame buffer at the specified 2D coordinates.
 * All pixels in the pixel array will be drawn in the same color.
 * 
 * @arg fb_x: The x index in the frame buffer.
 * @arg fb_y: The y index in the frame buffer.
 * @arg pixels: The pixels (chars) on the line to be drawn.
 * @arg width: The length of the line to be rendered.
 * @arg color: The color of all the characters to be rendered.
 * @arg frame_buffer: The frame buffer to draw to.
 * @arg current_width: The current width of a line in the frame buffer.
 */
void render_line_vert(int fb_x, int fb_y, char *pixels, int width, 
    Color_Pair color, FrameBuffer *frame_buffer, int current_width);

#endif