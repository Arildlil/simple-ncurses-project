#include "unit_surfaces.h"



static boolean inited = FALSE;

static void check_init() {
    if (inited == FALSE) {
        inited = TRUE;
        UnitImages_init();
    }
}

Surface* UnitSurface_archer(Surface *surface, int x, int y) {
    check_init();
    return Surface_init_image(surface, &UNIT_IMAGE_ARCHER, x, y, NULL);
}

Surface* UnitSurface_swordman(Surface *surface, int x, int y) {
    check_init();
    return Surface_init_image(surface, &UNIT_IMAGE_SWORDMAN, x, y, NULL);
}

Surface* UnitSurface_spearman(Surface *surface, int x, int y) {
    check_init();
    return Surface_init_image(surface, &UNIT_IMAGE_SPEARMAN, x, y, NULL);
}