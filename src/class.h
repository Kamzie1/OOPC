#ifndef CLASS_H
#define CLASS_H
#include "utils.h"
#include <stdarg.h>
#include <stdlib.h>

#define class typedef struct{\

#define end_class(NAME)\
    }NAME;

#define INIT_IMPL0(CLASS, NAME) CLASS NAME;
#define INIT_IMPL1(CLASS, NAME) __attribute__((cleanup(Destroy##CLASS))) CLASS NAME;
#define CALL_INIT(DESTRUCTOR, CLASS, NAME) EXPAND(INIT_IMPL, DESTRUCTOR)(CLASS, NAME)

#define init_class(CLASS, NAME, ...)\
    CALL_INIT(DESTRUCTOR_##CLASS, CLASS, NAME)\
    CONSTRUCT_FROM_TWO_MACROS(CONSTRUCTOR_##CLASS, VA_LEN(__VA_ARGS__))((&NAME), ##__VA_ARGS__)

#define init_class_ptr(CLASS, NAME, ...)\
    CLASS *NAME = (CLASS*)malloc(sizeof(CLASS));\
    CONSTRUCT_FROM_TWO_MACROS(CONSTRUCTOR_##CLASS, VA_LEN(__VA_ARGS__))((NAME), ##__VA_ARGS__)

#define free_class(CLASS, NAME)\
    Destroy##CLASS(NAME);\
    free(NAME);

#define INHERIT(PARENT) PUBLIC_##PARENT


#endif
