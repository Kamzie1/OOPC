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
    __attribute__((cleanup(Destroy##CLASS))) CLASS NAME;

/* Internal — selects _INIT_IMPL0 or _INIT_IMPL1 based on the DESTRUCTOR value */
#define _DISPATCH_INIT(DESTRUCTOR, CLASS, NAME) \
    PP_CONCAT(_INIT_IMPL, DESTRUCTOR)(CLASS, NAME)

/* Declares and constructs a stack-allocated class object */
#define init_class(CLASS, NAME, ...)                                            \
    _DISPATCH_INIT(DESTRUCTOR_##CLASS, CLASS, NAME)                             \
    PP_CONCAT(CONSTRUCTOR_##CLASS, VA_COUNT(__VA_ARGS__))((&NAME), ##__VA_ARGS__)

/* Declares and constructs a heap-allocated class pointer */
#define init_class_ptr(CLASS, NAME, ...)                                        \
    CLASS *NAME = (CLASS *)malloc(sizeof(CLASS));                               \
    PP_CONCAT(CONSTRUCTOR_##CLASS, VA_COUNT(__VA_ARGS__))((NAME), ##__VA_ARGS__)

/* Internal — no destructor do nothing, used when DESTRUCTOR_ClassName = 0 */
#define _DESTROY_IMPL0(CLASS, NAME)

/* Internal — call DESTROYClassName, used when DESTRUCTOR_ClassName = 1 */
#define _DESTROY_IMPL1(CLASS, NAME) \
    Destroy##CLASS(&(NAME))

/* Internal — selects _DESTROY_IMPL0 or _DESTROY_IMPL1 based on the DESTRUCTOR value */
#define DISPATCH_DESTRUCTOR(DESTRUCTOR, CLASS, NAME) \
    PP_CONCAT(_DESTROY_IMPL, DESTRUCTOR)(CLASS, NAME)

/* Calls the custom destructor then frees a heap-allocated class pointer */
#define free_class(CLASS, NAME)                               \
    do {                                                      \
        DISPATCH_DESTRUCTOR(DESTRUCTOR_##CLASS, CLASS, *NAME);\
        free(NAME);                                           \
        NAME = NULL;                                          \
    } while (0);

/* Pastes PUBLIC_PARENT fields into the current class body (inheritance) */
#define INHERIT(PARENT) PUBLIC_##PARENT

#endif
