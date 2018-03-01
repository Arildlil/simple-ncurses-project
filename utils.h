#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

typedef int boolean;

#ifndef FALSE
    #define FALSE 0
#endif
#ifndef TRUE
    #define TRUE 1
#endif

#if defined DEBUG
    #define dnprintf(format) printf(format)
    #define dprintf(format, expr) printf(format, expr)
#else
    #define dnprintf(format) 
    #define dprintf(format, expr) 
#endif

#endif