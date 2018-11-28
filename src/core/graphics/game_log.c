#include "../../include/game_log.h"
#include "../../include/rendering.h"
#include "rendering_internals.h"
#include "../../include/colors.h"

#include <assert.h>

#define MAX_LINES 6
#define MAX_LINE_LENGTH 40
#define LOG_HEIGHT MAX_LINES+2
#define LOG_WIDTH MAX_LINE_LENGTH+2



/* ----- | Function Prototypes | ----- */

static void add_line(char *string);



/* ----- | Static Variables | ------ */

static char lines[MAX_LINES][MAX_LINE_LENGTH+1] = {0};
static int line_index = 0;

static const char pixel_horizontal = '=';
static const char pixel_vertical = '|';
static const Color_Pair border_color = COLOR_PAIR_GREEN;
static const Color_Pair text_color = COLOR_PAIR_TEXT_BW;



/* ----- | Functions | ----- */

void GameLog_draw() {
    int screen_height = Rendering_get_screen_height();
    int screen_width = Rendering_get_screen_width();
    int menu_bottom_height = Rendering_get_menu_bottom_height();
    //int menu_bottom_width = Rendering_get_menu_bottom_width();
    
    int start_x = screen_width - LOG_WIDTH;
    int start_y = screen_height - menu_bottom_height;
    int end_x = screen_width - 1;
    int bottom_y = start_y + menu_bottom_height - 1;

    FrameBuffer *frame_buffer = get_frame_buffer();

    /* Top border */
    render_line_char(start_x, start_y, pixel_horizontal, LOG_WIDTH, border_color,
        frame_buffer, screen_width);
    /* Bottom border */
    render_line_char(start_x, bottom_y, pixel_horizontal, LOG_WIDTH, border_color,
        frame_buffer, screen_width);
    /* Vertical borders */
    render_line_vert_char(end_x, start_y, pixel_vertical, LOG_HEIGHT, border_color,
        frame_buffer, screen_width);
    render_line_vert_char(start_x, start_y, pixel_vertical, LOG_HEIGHT, border_color,
        frame_buffer, screen_width);

    /* Write strings */
    int i, count;
    for (count = 0, i = line_index; count < MAX_LINES; count++, i = (i + 1) % MAX_LINES) {
        if (lines[i][0] != '\0') {
            render_line(start_x + 1, bottom_y - 1 - count, lines[i], strlen(lines[i]), 
                text_color, frame_buffer, screen_width);
        }
    }
}

void GameLog_println(char *string) {
    assert(string);

    add_line(string);
}

static void add_line(char *string) {
    int cur_index = line_index;
    strncpy(lines[cur_index], string, MAX_LINE_LENGTH);
    lines[cur_index][MAX_LINE_LENGTH] = '\0';
    line_index = (line_index + 1) % MAX_LINES;
}