#include"OOP.h"
#include <stdlib.h>
#include <string.h>  
#include <stdio.h> 

void init_class(class* self, class* schema)
{
    if(schema == NULL){
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
    else{
        self->var_count = schema->var_count;
        self->func_count = schema->func_count;
        self->func_names = (char**)malloc(sizeof(char*)*self->func_count);
        self->var_names = (char**)malloc(sizeof(char*)*self->var_count);
        self->variables = (variable*)malloc(sizeof(variable)*self->var_count);
        self->functions = (function**)malloc(sizeof(function*)*self->func_count);
        for(size_t i=0;i<self->var_count;i++){
            *(self->var_names+i) = (char*)malloc(strlen(*(schema->var_names+i))+1);
            strcpy(*(self->var_names+i), *(schema->var_names+i));
            *(self->variables+i) = *(schema->variables+i);
        }
        for(size_t i=0;i<self->func_count;i++){
            *(self->func_names+i) = (char*)malloc(strlen(*(schema->func_names+i))+1);
            strcpy(*(self->func_names+i), *(schema->func_names+i));
            *(self->functions+i) = *(schema->functions+i);
        }
        self->parent = schema->parent;
        self->constructor = schema->constructor;
        self->destructor = schema->destructor;
        self->type = schema->type;
    }
   
}

void inherit(class* self, class* parent){
    if(parent->type == sealed){
        printf("[ERROR] cannot inherit from sealed class\n");
        return;
    }
    self->parent = parent;
    for(size_t i=0;i<parent->var_count;i++){
        add_variable(self, parent->variables[i].size, parent->var_names[i]);
    }
}

void add_function(class* self, void* func, size_t num_param, char* name, func_type type){
    class* current = self;
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
    new_func->num_param = num_param;
    self->func_count++;

    self->functions = (function**)realloc(self->functions, sizeof(function*) * self->func_count);
    *(self->functions+self->func_count-1) = new_func;

    self->func_names = (char**)realloc(self->func_names, sizeof(char*)*self->func_count);
    *(self->func_names+self->func_count-1) = (char*)malloc(strlen(name)+1);
    strcpy(*(self->func_names+self->func_count-1), name);
    new_func->type = type;
}

void call_function(class* self, char* name, void* args){
    class* current = self;  
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
    fptr(args, (*func)->num_param);
    return;
}

void add_variable(class* self, size_t size, char* name){
    char** pomo = self->var_names;
    for(size_t i=0;i<self->var_count;i++){
        if(strcmp(*pomo, name) == 0){
            printf("[ERROR] variable name already in use\n");
            return;
        }    
        pomo++;
    }
    variable* new_var = (variable*)malloc(sizeof(variable));
    new_var->id = self->var_count;
    new_var->size = size;
    new_var->data = malloc(size);
    self->var_count++;

    self->variables = (variable*)realloc(self->variables, sizeof(variable)*self->var_count);
    *(self->variables+self->var_count-1) = *new_var;


    self->var_names = (char**)realloc(self->var_names, sizeof(char*) * self->var_count);
    *(self->var_names+self->var_count-1) = (char*)malloc(strlen(name)+1);
    strcpy(*(self->var_names+self->var_count-1), name);
}

void* get_variable(class* self, char* name){
    char** pomo = self->var_names;
    int id = -1;
    for(size_t i=0;i<self->var_count;i++){
        if(strcmp(*pomo, name) == 0){
            id = i;
        }    
        pomo++;
    }
    if(id == -1){
        if(self->parent != NULL){
            return get_variable(self->parent, name);
        }
        else{
            printf("[ERROR] there is no such variable\n");
            return NULL;
        }
    }

    variable* var = self->variables;
    while(id!=var->id){
        var++;
    }
    
    return var->data;
}

