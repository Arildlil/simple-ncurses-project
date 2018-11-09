#ifndef UNIT_IMAGES_H
#define UNIT_IMAGES_H

#define UNIT_IMAGE_WIDTH 3
#define UNIT_IMAGE_HEIGHT 3

#include "image.h"
#include "utils.h"
#include "curses.h"

extern const char *UNIT_STRING_ARCHER[];
extern const char *UNIT_STRING_SWORDMAN[];
extern const char *UNIT_STRING_SPEARMAN[];

extern Image UNIT_IMAGE_ARCHER;
extern Image UNIT_IMAGE_SWORDMAN;
extern Image UNIT_IMAGE_SPEARMAN;
extern Image UNIT_IMAGE_PEASANT;



/*
 * Used to set the values above.
 */ 
void UnitImages_init();



#endif