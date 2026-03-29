#include "vtable.h"
#include "stdlib.h"

void register_vtable(vtable *vptr, func_t *func){
    size_t id = vptr->size;
    if (vptr->size == vptr->capacity){
        vptr->capacity *=2;
        vptr->func = realloc(vptr->func, vptr->capacity * sizeof(func_t*));
    }
    vptr->size +=1;
    memmove(vptr->func + id*sizeof(func_t*), func, sizeof(func_t*));
}

void init_vtable(vtable *vptr, void *class_ptr){
    vptr->class_ptr = class_ptr;
    vptr->size = 0;
    vptr->capacity = 0;
    vptr->func = 0;
}
