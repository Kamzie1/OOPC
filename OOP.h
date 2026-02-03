#pragma once
#include <stddef.h>

typedef enum {virtual, non_virtual} func_type;
typedef enum {sealed, unsealed} class_type;

#define UNUSED(x) (void)(x)

typedef struct
{
    void* body;
    int id;
    func_type type;
}function;

typedef struct{
    size_t size;
    int id;
}variable_schema;

typedef struct{
    size_t size;
    void* data;
}variable;

typedef struct class_schema{
    char** func_names;
    char** var_names;
    variable_schema* variables;
    function** functions;
    struct class_schema* parent;
    
    void* constructor;
    void* destructor;

    size_t var_count;
    size_t func_count;
    class_type type;
}class_schema;

typedef struct class
{
    class_schema* base;
    variable* variables;
}class;

void init_class(class* self, class_schema* schema);
void init_schema(class_schema* self);
void inherit(class_schema* self, class_schema* parent);
void add_function(class_schema* self, void* func, char* name, func_type type);
void add_variable(class_schema* self, size_t size, char* name);
void call_function(class* self, char* name, void* args, size_t num_param, size_t size_of_param);
variable* get_variable(class* self, char* name);
void free_class(class* self);
void free_schema(class_schema* self);

