#ifndef VTABLE_H
#define VTABLE_H

typedef void (*func_t)(void);

typedef struct{
    func_t *func;
    size_t size;
    size_t capacity;
    void *class_ptr;
}vtable;

void init_vtable(vtable *vptr, void *class_ptr);
void register_vtable(vtable *vptr, void *func);

#endif
