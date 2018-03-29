#ifndef RENDERING_H
#define RENDERING_H

#include "gameobject.h"
#include "terrain.h"

boolean Rendering_init(size_t width, size_t height);
void Rendering_add_background(Map *map, int center_x, int center_y);
void Rendering_render_object(GameObject *object);

#endif 