#include "unit_images.h"
#include "image.h"


const char *UNIT_STRING_ARCHER[] = {
    "|\\ ",
    "+->", 
    "|/ "
};

const char *UNIT_STRING_SWORDMAN[] = {
    " | ", 
    " | ",
    "-+-"
};

const char *UNIT_STRING_SPEARMAN[] = {
    " ^ ",
    " | ",
    " | "
};

Image UNIT_IMAGE_ARCHER;
Image UNIT_IMAGE_SWORDMAN;
Image UNIT_IMAGE_SPEARMAN;

void UnitImages_init() {
    Image_init_2D(&UNIT_IMAGE_ARCHER, UNIT_IMAGE_WIDTH, UNIT_IMAGE_HEIGHT, UNIT_STRING_ARCHER);
    Image_init_2D(&UNIT_IMAGE_SWORDMAN, UNIT_IMAGE_WIDTH, UNIT_IMAGE_HEIGHT, UNIT_STRING_SWORDMAN);
    Image_init_2D(&UNIT_IMAGE_SPEARMAN, UNIT_IMAGE_WIDTH, UNIT_IMAGE_HEIGHT, UNIT_STRING_SPEARMAN);
}