# OOPC

OOPC is an easy-to-use C framework for working with object-oriented programming concepts.

# Features

## Defining a Class

Class definitions are placed within the `class` / `end_class` tags:

```C
class
    // definition of Base class
end_class(Base)
```

To declare fields, create a macro following the format `PUBLIC_[ClassName]` and call it inside the class body:

```C
class
    #define PUBLIC_Base \
        int a_;
    PUBLIC_Base
end_class(Base)
```

## Creating a Class Object

```C
init_class(Base, base)         // initializes 'base' on the stack as a Base object
init_class_ptr(Base, base_ptr) // initializes 'base_ptr' as a Base* on the heap
```

## Constructors

Constructors are optional. To define one, create a macro with the format:

```
CONSTRUCTOR_[ClassName][NumberOfArgs](self, ...)
```

For example:

```C
#define CONSTRUCTOR_Base1(self, a) \
    self->a_ = a;

#define CONSTRUCTOR_Abc3(self, a, b, c) \
    self->a_ = a; \
    self->b_ = b; \
    self->c_ = c;

#define CONSTRUCTOR_Abc0(self) \
    self->a_ = 0; \
    self->b_ = 0; \
    self->c_ = 1;
```

Constructors are called automatically inside `init_class()` and `init_class_ptr()`. The correct constructor is selected based on the number of arguments passed:

```C
init_class(Abc, abc_empty)    // calls CONSTRUCTOR_Abc0
init_class(Abc, abc, 1, 2, 3) // calls CONSTRUCTOR_Abc3
```

> **Note:** Constructor argument names must differ from the class field names
> (e.g. use `self->a_ = a` rather than `self->a_ = a_`, where the parameter
> would shadow the field name and cause incorrect macro substitution).
> Additionally, two constructors with the same number of arguments but different
> types cannot be defined — unlike C++ overloading.

## Destructors

Every class declaration must define `DESTRUCTOR_[ClassName]` as either `0` or `1`:

- `0` — uses the default destructor (no custom cleanup needed)
- `1` — signals that you will provide a custom destructor (required when the class allocates memory internally)

### Default Destructor

```C
class
    #define PUBLIC_Base \
        int a_;
    PUBLIC_Base
    #define DESTRUCTOR_Base 0
end_class(Base)
```

### Custom Destructor

A custom destructor is a `void` function named `Destroy[ClassName]` that takes a pointer to the class object:

```C
class
    #define PUBLIC_String \
        char  *data_;     \
        size_t size_;     \
        size_t capacity_;
    PUBLIC_String
    #define DESTRUCTOR_String 1
end_class(String)

void DestroyString(String *self) {
    free(self->data_);
    self->data_ = NULL;
}
```

### How Destructors Work

**Heap-allocated objects** must be freed manually using `free_class` instead of `free`:

```C
free_class(String, str_ptr)
```

If `DESTRUCTOR_String` is `0`, this simply calls `free(str_ptr)`. If it is `1`, it first calls your custom destructor and then frees the pointer.

**Stack-allocated objects** are handled automatically. The framework uses the GCC/Clang `__attribute__((cleanup(...)))` extension to call the destructor when the variable goes out of scope, so you never need to free it manually.

## Inheritance

OOPC supports multiple inheritance. To inherit from a parent class, use `INHERIT(ClassName)` inside the child class body:

```C
INHERIT(ClassName)
```

### Example

```C
class
    #define PUBLIC_Base \
        int a_;
    PUBLIC_Base
    #define DESTRUCTOR_Base 0
end_class(Base)

class
    INHERIT(Base)
    #define PUBLIC_Object \
        int b_; \
        int c_;
    PUBLIC_Object
    #define DESTRUCTOR_Object 0
end_class(Object)
```

# Demo

Demos are available in the `demo` folder.