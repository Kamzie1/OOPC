#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

#define offset(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)

#define container_of(ptr, TYPE, MEMBER) ({\
    const typeof(((TYPE*)0)->MEMBER) *_ptr = (ptr);\
    (TYPE*)((char*)(ptr) - offset(TYPE, MEMBER));\
})


#define EXPAND(NAME, N) NAME##N
#define CONSTRUCT_FROM_TWO_MACROS(NAME, N) EXPAND(NAME, N)

#define GET_NTH_ARG(_1, _2, _3, _4,_5, _6, N, ...) N
#define VA_LEN(...) GET_NTH_ARG(-1, ##__VA_ARGS__, 5, 4, 3, 2, 1, 0)

#endif
