#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

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

/* Source: http://www.c-faq.com/style/strcmp.html. */
#define StrRel(s1, op, s2) (strcmp(s1, s2) op 0)

#endif