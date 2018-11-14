#ifndef UNIT_SURFACES_H
#define UNIT_SURFACES_H

#include "unit_images.h"
#include "include/surface.h"

/* (All functions above)
 *
 * Fills out the surface object to create a unit.
 * @arg surface: The surface object to fill out.
 * @arg x: The x-coordinate to place it.
 * @arg y: The y-coordinate to place it.
 * @return: The object that just got initialized.
 */
Surface* UnitSurface_archer(Surface *surface, int x, int y);
Surface* UnitSurface_swordman(Surface *surface, int x, int y);
Surface* UnitSurface_spearman(Surface *surface, int x, int y);
Surface* UnitSurface_peasant(Surface *surface, int x, int y);

#endif