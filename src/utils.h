#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

#define MEMBER_OFFSET(TYPE, MEMBER) \
    ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(PTR, TYPE, MEMBER) ({                              \
    const typeof(((TYPE *)0)->MEMBER) *_ptr = (PTR);                    \
    (TYPE *)((char *)(PTR) - MEMBER_OFFSET(TYPE, MEMBER));              \
})

/* Internal helper — raw token paste, no expansion */
#define _PP_CONCAT(A, B)  A##B
/* Public — forces expansion of A and B before pasting */
#define PP_CONCAT(A, B)   _PP_CONCAT(A, B)

/* Internal helper — selects the Nth argument */
#define _VA_NTH_ARG(_1, _2, _3, _4, _5, _6, N, ...) N
/* Counts the number of variadic arguments (0–5) */
#define VA_COUNT(...) _VA_NTH_ARG(-1, ##__VA_ARGS__, 5, 4, 3, 2, 1, 0)

#endif