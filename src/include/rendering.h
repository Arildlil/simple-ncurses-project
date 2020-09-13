#ifndef RENDERING_H
#define RENDERING_H

#include "gameobject.h"
#include "map.h"



typedef struct RenderCoordinateBorders {
    int left_x;
    int right_x;
    int top_y;
    int bottom_y;
    size_t index_left_x;
    size_t index_right_x;
    size_t index_top_y;
    size_t index_bottom_y;
} RenderCoordinateBorders;



boolean Rendering_init(size_t width, size_t height);
void Rendering_fill_framebuffer(Map *map, int center_x, int center_y, GameObject objects[], int num_elements);
void Rendering_render_frame();

void Rendering_convert_coordinates(Map *map, int half_screen_width, int half_screen_height, 
    int center_x, int center_y, RenderCoordinateBorders *borders);

#endif 
