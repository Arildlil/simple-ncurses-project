#include "include/utils.h"

#include <time.h>
#include <stdlib.h>

static int inited = 0;

int random_in_range(int from, int to) {
    if (!inited) {
        srand(time(0));
    }
    
    inited = 1;
    
    return (rand() % (to - from)) + from;
}
