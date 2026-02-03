#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OOP.h"

void print(void* args, size_t num_param){
    printf("Animal speaks!\n");
}
void print_dog(void* args, size_t num_param){
    printf("Dog barks!\n");
}

void print_cat(void* args, size_t num_param){
    printf("Cat meows!\n");
}

void info(void* args, size_t num_param){
    int age = *((int*)args);
    printf("Animal age: %d\n", age);
}

void Animal(class* base){
    init_class(base, NULL);
    add_variable(base, sizeof(int), "age");
    add_function(base, print, 0, "speak", virtual);
    add_function(base, info, 1, "info", non_virtual);
}

void Dog(class* derived, class* base){
    init_class(derived, NULL);
    inherit(derived, base);
    add_function(derived, print_dog, 0, "speak", non_virtual);
}

void Cat(class* derived, class* base){
    init_class(derived, NULL);
    inherit(derived, base);
    add_function(derived, print_cat, 0, "speak", non_virtual);
}

int main(){
    class Animal_class;
    Animal(&Animal_class);
    class Dog_class;
    Dog(&Dog_class, &Animal_class);
    class Cat_class;
    Cat(&Cat_class, &Animal_class);

    class my_dog;
    init_class(&my_dog, &Dog_class);
    * (int*)get_variable(&my_dog, "age") = 5;

    class my_cat;
    init_class(&my_cat, &Cat_class);
    * (int*)get_variable(&my_cat, "age") = 3;

    call_function(&my_dog, "speak", NULL);
    call_function(&my_cat, "speak", NULL);

    call_function(&my_dog, "info", get_variable(&my_dog, "age"));
    call_function(&my_cat, "info", get_variable(&my_cat, "age"));
    return 0;
}