#ifndef RENDERING_H
#define RENDERING_H

#include "gameobject.h"

boolean Rendering_init(size_t width, size_t height);
void Rendering_render_object(GameObject *object);

#endif 