#ifndef CLASS_H
#define CLASS_H
#include "utils.h"
#include "vtable.h"

#define class(NAME) typedef struct{
#define end_class(NAME) }NAME;

#define DECLARE(TYPE, NAME) TYPE NAME;
#define DECLARE_FUNCTION(RETURN_TYPE, NAME, ...) RETURN_TYPE (*NAME)(__VA_ARGS__);
#define REGISTER(RETURN_TYPE, NAME, ...) \
    register_vtable(vptr, NAME);\


#define class_init(CLASS, NAME);\
    vtable *vptr;\
    init_vtable(vptr,(void*)& NAME);\
    FOR_FUNCTION_OF_##CLASS( REGISTER )


#define CALL_FUNCTION(object, id)\
    vtable *tmp_vptr = container_of((void*)object, vtable, class_ptr);\
    ((func_t*)(vptr->func+id*sizeof(func_t*)))();

#endif
