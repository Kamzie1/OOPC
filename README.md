# I added OOP to C

### The syntax looks like this

##### You create a class schema. Note that each function must only have a pointer to its args, number of args and size of a single argument
```C
void print(void* args, size_t num_param, size_t size_of_param){
    UNUSED(args);
    UNUSED(num_param); // you can type this so that compiler doesn't complain if you don't use these variables
    UNUSED(size_of_param);
    printf("Animal speaks!\n");
}

void info(void* args, size_t num_param, size_t size_of_param){
    UNUSED(args);
    UNUSED(num_param);
    UNUSED(size_of_param);
    int age = *((int*)args);
    printf("Animal age: %d\n", age);
}

void AnimalConstructor(class* self){
    *(int*)get_variable(self, "age")->data = 1;
}

void AnimalDestructor(class* self){
    UNUSED(self);
    // Cleanup if needed
}

void Animal(class_schema* base){
    init_schema(base); // initialize a schema
    add_variable(base, sizeof(int), "age"); // add vatriable age of size int
    add_function(base, print, "speak", virtual); // add virtual(meaning it can be overwritten by children) function speak that behaves like defined above print function
    add_function(base, info, "info", non_virtual);
    base->constructor = AnimalConstructor;
    base->destructor = AnimalDestructor;
    // a class is nonsealed unless you say otherwise: base->type = sealed 
}
```

#### Then you make even more class schemas
```C
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

void Dog(class_schema* self, class_schema* base){
    init_schema(self);
    inherit(self, base); // class_schema inherits from base class_schema
    add_function(self, print_dog, "speak", non_virtual);
    self->type = sealed; // Dog_class is a sealed class meaning you can't inherit from it
}

void Cat(class_schema* self, class_schema* base){
    init_schema(self);
    inherit(self, base);
    add_function(self, print_cat, "speak", non_virtual);
}
```

##### Now you need one instance of a schema and then with schema you can create as many class instances as you want
```C
int main(){
    class_schema Animal_class;
    Animal(&Animal_class);
    class_schema Dog_class;
    Dog(&Dog_class, &Animal_class);
    class_schema Cat_class;
    Cat(&Cat_class, &Animal_class);

    class my_animal;
    init_class(&my_animal, &Animal_class);
    call_function(&my_animal, "info", get_variable(&my_animal, "age")->data, 1, sizeof(int));//notice that constructor is called within init_class

    class my_dog;
    init_class(&my_dog, &Dog_class);
    * (int*)get_variable(&my_dog, "age")->data = 5;

    class my_cat;
    init_class(&my_cat, &Cat_class);
    * (int*)get_variable(&my_cat, "age")->data = 3;
    call_function(&my_dog, "speak", NULL, 0, 0);//notice how we achived polymorphis using virtual functions
    call_function(&my_cat, "speak", NULL, 0, 0);

    call_function(&my_dog, "info", get_variable(&my_dog, "age")->data, 1, sizeof(int));
    call_function(&my_cat, "info", get_variable(&my_cat, "age")->data, 1, sizeof(int));
    free_class(&my_animal);//you should free_class a class when you no longer use it
    free_class(&my_dog);
    free_class(&my_cat);
    free_schema(&Animal_class);//you should also free a class_schema
    free_schema(&Dog_class);
    free_schema(&Cat_class);
    return 0;
}
```

# Docs

## Variable
```C
typedef struct{
    size_t size;
    void* data;
}variable;
```

### init_class
creates an instance of a class out of a class schema and calls a constructor of that class
```C
void init_class(class* self, class_schema* schema);
```
### init_schema
creates an empty schema for a class
```C
void init_schema(class_schema* self);
```
### inherit
inherits variables and functions from parent, notice that constructors aren't inherited
```C
void inherit(class_schema* self, class_schema* parent);
```
### add_function
adds function to a schema, fails if function with the same name already exists.
`void* func` - pointer to already defined function. Remember that the function needs to be void and have exactly three parameters: `void* args`, `size_t num_param` and `size_t size_of_param`
`char* name` - name of a function
`func_type` - virtual, nonvirtual
```C
void add_function(class_schema* self, void* func, char* name, func_type type);
```
### add_variable
adds variable to a schema, fails if variable with the same name already exists.
`size_t size` - size of a new variable
`char* name` - name of a new variable
```C
void add_variable(class_schema* self, size_t size, char* name);
```
### call_function
call a function on a class instance, fails if a function with specified name doesn't exist
`char* name` - name of a function to call
`void* args` - pointer to function arguments you should define a way to handle args in definition of a function
`size_t num_param` - number of params passed in args
`size_t size_of_param` - sizeof parameter 
```C
void call_function(class* self, char* name, void* args, size_t num_param, size_t size_of_param);
```
### get_variable
returns a pointer to variable of a given name, fails if there is no such variable
```C
void* get_variable(class* self, char* name);
```
### free_class
frees an instance of a class, deconstructor is called if it exists
```C
void free_class(class* self);
```
### free_schema
frees an entire schema
```C
void free_schema(class_schema* self);
```