#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

#define offset(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)

#define container_of(ptr, TYPE, MEMBER) ({\
    const typeof(((TYPE*)0)->MEMBER) *_ptr = (ptr);\
    (TYPE*)((char*)(ptr) - offset(TYPE, MEMBER));\
})

#endif
