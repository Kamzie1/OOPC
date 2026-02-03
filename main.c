#include<stdio.h>
#include "OOP.h"

void print(void* args, size_t num_param, size_t size_of_param){
    UNUSED(args);
    UNUSED(num_param);
    UNUSED(size_of_param);
    printf("Animal speaks!\n");
}
void print_dog(void* args, size_t num_param, size_t size_of_param){
    UNUSED(args);
    UNUSED(num_param);
    UNUSED(size_of_param);
    printf("Dog barks!\n");
}

void print_cat(void* args, size_t num_param, size_t size_of_param){
    UNUSED(args);
    UNUSED(num_param);
    UNUSED(size_of_param);
    printf("Cat meows!\n");
}

void info(void* args, size_t num_param, size_t size_of_param){
    UNUSED(num_param);
    UNUSED(size_of_param);
    int age = *((int*)args);
    printf("Animal age: %d\n", age);
}

void AnimalConstructor(class* self){
    *(int*)get_variable(self, "age") = 1;
}

void AnimalDestructor(class* self){
    UNUSED(self);
}

void Animal(class_schema* base){
    init_schema(base);
    add_variable(base, sizeof(int), "age");
    add_function(base, print, "speak", virtual);
    add_function(base, info, "info", non_virtual);
    base->constructor = AnimalConstructor;
    base->destructor = AnimalDestructor;
}

void Dog(class_schema* self, class_schema* base){
    init_schema(self);
    inherit(self, base);
    add_function(self, print_dog, "speak", non_virtual);
    self->type = sealed;
}

void Cat(class_schema* self, class_schema* base){
    init_schema(self);
    inherit(self, base);
    add_function(self, print_cat, "speak", non_virtual);
}

int main(){
    class_schema Animal_class;
    Animal(&Animal_class);
    class_schema Dog_class;
    Dog(&Dog_class, &Animal_class);
    class_schema Cat_class;
    Cat(&Cat_class, &Animal_class);

    class my_animal;
    init_class(&my_animal, &Animal_class);
    call_function(&my_animal, "info", get_variable(&my_animal, "age"), 1, sizeof(int));

    class my_dog;
    init_class(&my_dog, &Dog_class);
    * (int*)get_variable(&my_dog, "age") = 5;

    class my_cat;
    init_class(&my_cat, &Cat_class);
    * (int*)get_variable(&my_cat, "age") = 3;
    call_function(&my_dog, "speak", NULL, 0, 0);
    call_function(&my_cat, "speak", NULL, 0, 0);

    call_function(&my_dog, "info", get_variable(&my_dog, "age"), 1, sizeof(int));
    call_function(&my_cat, "info", get_variable(&my_cat, "age"), 1, sizeof(int));
    free_class(&my_animal);
    free_class(&my_dog);
    free_class(&my_cat);
    free_schema(&Animal_class);
    free_schema(&Dog_class);
    free_schema(&Cat_class);
    return 0;
}