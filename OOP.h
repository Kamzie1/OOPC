#pragma once
#include <stddef.h>

typedef enum { private, public, protected } scope;
typedef enum {virtual, non_virtual} func_type;
typedef enum {sealed, unsealed} class_type;

typedef struct
{
    size_t num_param;
    void* body;
    int id;
    func_type type;
}function;

typedef struct{
    size_t size;
    void* data;
    int id;
}variable;

typedef struct class
{
    char** func_names;
    char** var_names;
    variable* variables;
    function** functions;
    struct class* parent;
    
    void* constructor;
    void* destructor;

    size_t var_count;
    size_t func_count;
    class_type type;
}class;

void init_class(class* self, class* schema);
void inherit(class* self, class* parent);
void add_function(class* self, void* func, size_t num_param, char* name, func_type type);
void add_variable(class* self, size_t size, char* name);
void call_function(class* self, char* name, void* args);
void* get_variable(class* self, char* name);

