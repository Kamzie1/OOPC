#include"OOP.h"
#include <stdlib.h>
#include <string.h>  
#include <stdio.h> 

void init_schema(class_schema* self)
{        
        if(self == NULL){
            printf("[ERROR] class schema cannot be NULL\n");
            return;
        }

        self->var_count = 0;
        self->func_count = 0;
        self->func_names = NULL;
        self->var_names = NULL;
        self->variables = NULL;
        self->functions = NULL;
        self->parent = NULL;
        self->constructor = NULL;
        self->destructor = NULL; 
        self->type = unsealed;
}

void init_class(class* self, class_schema* schema){
        if(self == NULL){
            printf("[ERROR] class cannot be NULL\n");
            return;
        }
        if(schema == NULL){
            printf("[ERROR] class schema cannot be NULL\n");
            return;
        }
        self->base = schema;
        self->variables = (variable*)malloc(sizeof(variable)*schema->var_count);
        for(size_t i=0;i<schema->var_count;i++){
            (self->variables+i)->data = malloc((schema->variables[i]).size);
            (self->variables+i)->size = (schema->variables[i]).size;}
        if(schema->constructor != NULL){
            void (*fptr)() = schema->constructor;
            fptr(self);
        }
}

void inherit(class_schema* self, class_schema* parent){
    if(parent->type == sealed){
        printf("[ERROR] cannot inherit from sealed class\n");
        return;
    }
    self->parent = parent;
    for(size_t i=0;i<parent->var_count;i++){
        add_variable(self, parent->variables[i].size, parent->var_names[i]);
    }
}

void add_function(class_schema* self, void* func, char* name, func_type type){
    class_schema* current = self;
    while(current != NULL){
        char** pomo = current->func_names;
        for(size_t i=0;i<current->func_count;i++){
            if(strcmp(*pomo, name) == 0 && current->functions[i]->type == non_virtual){
                printf("[ERROR] function name already in use\n");
                return;
            }    
            pomo++;
        }
        if(current->parent != NULL){
            current = current->parent;
        }
        else{
            break;
        }
    }
    function* new_func = (function*)malloc(sizeof(function));
    new_func->id = self->func_count;
    new_func->body = func;
    self->func_count++;

    self->functions = (function**)realloc(self->functions, sizeof(function*) * self->func_count);
    *(self->functions+self->func_count-1) = new_func;

    self->func_names = (char**)realloc(self->func_names, sizeof(char*)*self->func_count);
    *(self->func_names+self->func_count-1) = (char*)malloc(strlen(name)+1);
    strcpy(*(self->func_names+self->func_count-1), name);
    new_func->type = type;
}

void call_function(class* self, char* name, void* args, size_t num_param, size_t size_of_param){
    class_schema* current = self->base;  
    int id = -1;
    while(current!=NULL){
        char** pomo = current->func_names;
        for(size_t i=0;i<current->func_count;i++){
            if(strcmp(*pomo, name) == 0){
                id = i;
                break;
            }    
            pomo++;
        }
        if(id == -1){
            if(current->parent != NULL){
                current = current->parent;
            }
            else{
                printf("[ERROR] there is no such function\n");
                return;
            }
        }
        else{
            break;
        }
    }
    
    function** func = current->functions;
    while(id!=(*func)->id){
        func++;
    }
    
    void (*fptr)() = (*func)->body;
    fptr(args, num_param, size_of_param);
    return;
}

void add_variable(class_schema* self, size_t size, char* name){
    char** pomo = self->var_names;
    for(size_t i=0;i<self->var_count;i++){
        if(strcmp(*pomo, name) == 0){
            printf("[ERROR] variable name already in use\n");
            return;
        }    
        pomo++;
    }
    variable_schema new_var;
    new_var.size = size;
    self->var_count++;

    self->variables = (variable_schema*)realloc(self->variables, sizeof(variable_schema)*self->var_count);
    *(self->variables+self->var_count-1) = new_var;

    self->var_names = (char**)realloc(self->var_names, sizeof(char*) * self->var_count);
    *(self->var_names+self->var_count-1) = (char*)malloc(strlen(name)+1);
    strcpy(*(self->var_names+self->var_count-1), name);
}
void free_class(class* self) {
    if(self == NULL) return;
    if(self->base && self->base->destructor) {
        void (*dtor)(class*) = self->base->destructor;
        dtor(self);
    }
    for(size_t i=0; i<self->base->var_count; i++) {
        free(self->variables[i].data);
    }
    free(self->variables);
    self->base = NULL;
}

void free_schema(class_schema* self) {
    if(self == NULL) return;

    for(size_t i=0; i<self->var_count; i++) { 
        free(self->var_names[i]);
    }
    
    free(self->var_names);
    free(self->variables);

    for(size_t i=0; i<self->func_count; i++) {
        free(self->func_names[i]);
        free(self->functions[i]);
    }
    free(self->func_names);
    free(self->functions);

    self->parent = NULL;
    self->constructor = NULL;
    self->destructor = NULL;
}

variable* get_variable(class* self, char* name){
    char** pomo = self->base->var_names;
    int id = -1;
    for(size_t i=0;i<self->base->var_count;i++){
        if(strcmp(*pomo, name) == 0){
            id = i;
        }    
        pomo++;
    }
    if(id == -1){
        printf("[ERROR] there is no such variable\n");
        return NULL;
    }

    variable* var = self->variables;
    int var_id = 0;
    while(id!=var_id){
        var++;
        var_id++;
    }
    
    return var;
}

