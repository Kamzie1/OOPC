#ifndef CLASS_H
#define CLASS_H
#include "utils.h"
#include <stdarg.h>
#include <stdlib.h>

/* tags creating a new  type out of class declaration */
#define class           typedef struct {
#define end_class(NAME) } NAME;

/* Internal — plain stack declaration, used when DESTRUCTOR_ClassName = 0 */
#define _INIT_IMPL0(CLASS, NAME) \
    CLASS NAME;

/* Internal — stack declaration with scope-exit cleanup, used when DESTRUCTOR_ClassName = 1 */
#define _INIT_IMPL1(CLASS, NAME) \
    __attribute__((cleanup(DESTRUCTOR_##CLASS))) CLASS NAME;

/* Internal — selects _INIT_IMPL0 or _INIT_IMPL1 based on the DESTRUCTOR value */
#define _DISPATCH_INIT(DESTRUCTOR, CLASS, NAME) \
    CONCAT(_INIT_IMPL, DESTRUCTOR)(CLASS, NAME)

/* Declares and constructs a stack-allocated class object */
#define init_class(CLASS, NAME, ...)                                            \
    _DISPATCH_INIT(CUSTOM_DESTRUCTOR_##CLASS, CLASS, NAME)                             \
    DISPATCH_CONSTRUCTOR(CLASS, NAME, __VA_ARGS__)



/* Declares and constructs a heap-allocated class pointer */
#define init_class_ptr(CLASS, NAME, ...)                                        \
    CLASS *NAME = (CLASS *)malloc(sizeof(CLASS));                               \
    DISPATCH_CONSTRUCTOR_FOR_PTR(CLASS, NAME, __VA_ARGS__)


/* Constructs a function type(for dispatching constructors) based on types of arguments
 * e.g. CONSTRUCT_FUNCTION_TYPE(1, (double)1, (float)1) -> (void(*)(void*, int, double, float))0
 */
#define CONSTRUCT_FUNCTION_TYPE(...) \
    (void(*)(void* __VA_OPT__(,) FOR_EACH(typeof, ##__VA_ARGS__)))0

/*Generates an entry in _Generic for a constructor registered in a class */
#define GENERATE_GENERIC(func, ...)\
     void(*)(void* __VA_OPT__(,) ##__VA_ARGS__ ): func,

/*Genrates dynamic _Genric block for automated calling of a specific constructor */
#define DISPATCH_CONSTRUCTOR(CLASS, NAME, ...)\
    _Generic((CONSTRUCT_FUNCTION_TYPE(__VA_ARGS__)),  \
        FOR_CONSTRUCTORS_##CLASS(GENERATE_GENERIC)  \
        default: (void*)0                           \
    )((&NAME) __VA_OPT__(,) ##__VA_ARGS__ );

/*  DISPATCH_CONSTRUCTOR but for pointers */
#define DISPATCH_CONSTRUCTOR_FOR_PTR(CLASS, NAME, ...)\
    _Generic((CONSTRUCT_FUNCTION_TYPE(__VA_ARGS__)),  \
        FOR_CONSTRUCTORS_##CLASS(GENERATE_GENERIC)  \
        default: (void*)0                           \
    )((NAME) __VA_OPT__(,) ##__VA_ARGS__ );

/* Internal — no destructor do nothing, used when DESTRUCTOR_ClassName = 0 */
#define _DESTROY_IMPL0(CLASS, NAME)

/* Internal — call DESTROYClassName, used when DESTRUCTOR_ClassName = 1 */
#define _DESTROY_IMPL1(CLASS, NAME) \
    DESTRUCTOR_##CLASS(&(NAME))

/* Internal — selects _DESTROY_IMPL0 or _DESTROY_IMPL1 based on the DESTRUCTOR value */
#define DISPATCH_DESTRUCTOR(DESTRUCTOR, CLASS, NAME) \
    CONCAT(_DESTROY_IMPL, DESTRUCTOR)(CLASS, NAME)

/* Calls the custom destructor then frees a heap-allocated class pointer */
#define free_class(CLASS, NAME)                               \
    do {                                                      \
        DISPATCH_DESTRUCTOR(CUSTOM_DESTRUCTOR_##CLASS, CLASS, *NAME);\
        free(NAME);                                           \
        NAME = NULL;                                          \
    } while (0);

/* Pastes PUBLIC_PARENT fields into the current class body (inheritance) */
#define INHERIT(PARENT) PUBLIC_##PARENT

#endif
