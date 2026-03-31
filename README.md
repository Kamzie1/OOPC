# OOPC

OOPC is an easy-to-use C framework for working with object-oriented programming concepts.

# Overview

The library relies heavily on macros, so preprocessed code is shown as an example of how things work under the hood.

# Features

## Defining a Class

Class definitions are placed within the `class` / `end_class` tags:

<table>
<tr>
<th>Before</th>
<th>After</th>
</tr>
<tr>
<td>

```C
class
    // definition of Base class
end_class(Base)
```

</td>
<td>

```C
typedef struct {
    // definition of Base class
} Base;
```

</td>
</tr>
</table>

To declare fields, create a macro following the format `PUBLIC_[ClassName]` and call it inside the class body:

<table>
<tr>
<th>Before</th>
<th>After</th>
</tr>
<tr>
<td>

```C
class
    #define PUBLIC_Base \
        int a_;
    PUBLIC_Base
end_class(Base)
```

</td>
<td>

```C
typedef struct {
    int a_;
} Base;
```

</td>
</tr>
</table>

## Creating a Class Object

<table>
<tr>
<th>Before</th>
<th>After</th>
</tr>
<tr>
<td>

```C
init_class(Base, base)         // stack-allocated Base object
init_class_ptr(Base, base_ptr) // heap-allocated Base pointer
```

</td>
<td>

```C
Base base;
Base *base_ptr;
```

</td>
</tr>
</table>

## Constructors

Constructors are optional. To define one, create a `void` function with a pointer to the class type as the first parameter (conventionally named `self` or `this`), followed by as many arguments as needed.

For example:

```C
static void construct_abc3(Abc *self, float a, int b, double c) {
    self->a_ = a;
    self->b_ = b;
    self->c_ = c;
}

// In C these are just regular functions — name them uniquely to avoid conflicts.
static void construct_abc0(Abc *self) {
    self->a_ = 0;
    self->b_ = 0;
    self->c_ = 0;
}

static void construct_abc1_float(Abc *self, float a) {
    self->a_ = a;
    self->b_ = 0;
    self->c_ = 0;
}

static void construct_abc1_int(Abc *self, int b) {
    self->a_ = 0;
    self->b_ = b;
    self->c_ = 0;
}
```

After defining these functions **below** the class definition, register them as constructors to enable dynamic dispatch. The registration macro follows this format:

```C
#define FOR_CONSTRUCTORS_[CLASSNAME](REGISTER) \
    REGISTER(function_name, [argument types])

// If you are familiar with X-macros, you will know that REGISTER can be any
// identifier — the name REGISTER is chosen to make the intent clear.
```

For example:

```C
#define FOR_CONSTRUCTORS_Abc(REGISTER)           \
    REGISTER(construct_abc0)                     \
    REGISTER(construct_abc3, float, int, double) \
    REGISTER(construct_abc1_int,   int)          \
    REGISTER(construct_abc1_float, float)
```

Constructors are called automatically inside `init_class()` and `init_class_ptr()`. The correct overload is selected based on the types of the arguments passed:

```C
init_class(Abc, abc_empty)                   // calls construct_abc0
init_class(Abc, abc, (float)1, 2, (double)3) // calls construct_abc3
init_class(Abc, abc, 1)                      // calls construct_abc1_int
init_class(Abc, abc, (float)1)               // calls construct_abc1_float
```

> **Note:** Unlike C++, C does not perform implicit type casting. If you are unsure
> whether an expression has the correct type (e.g. a string literal has type `char[N]`,
> not `char *`) cast it explicitly: `(char *)"hello"`.

The table below shows what `init_class(Abc, abc, (float)1, 2, (double)3)` expands to after preprocessing:

<table>
<tr>
<th>Before</th>
<th>After</th>
</tr>
<tr>
<td>

```C
init_class(Abc, abc, (float)1, 2, (double)3)
```

</td>
<td>

```C
Abc abc;
_Generic(
    ((void (*)(void *, typeof((float)1), typeof(2), typeof((double)3)))0),
    void (*)(void *):                    construct_abc0,
    void (*)(void *, float, int, double): construct_abc3,
    void (*)(void *, int):               construct_abc1_int,
    void (*)(void *, float):             construct_abc1_float,
    default: (void *)0
)((&abc), (float)1, 2, (double)3);
```

</td>
</tr>
</table>

## Destructors

Every class declaration must define `CUSTOM_DESTRUCTOR_[ClassName]` as either `0` or `1`:

- `0` — uses the default destructor (no custom cleanup needed)
- `1` — signals that you will provide a custom destructor (required when the class allocates memory internally)

### Default Destructor

```C
class
    #define PUBLIC_Base \
        int a_;
    PUBLIC_Base
    #define CUSTOM_DESTRUCTOR_Base 0
end_class(Base)
```

### Custom Destructor

A custom destructor is a `void` function that takes a pointer to the class object as its argument. To register it, define `DESTRUCTOR_[ClassName]` to expand to the function name. The function should be declared below the class definition.

```C
class
    #define PUBLIC_String \
        char  *data_;     \
        size_t size_;     \
        size_t capacity_;
    PUBLIC_String
    #define CUSTOM_DESTRUCTOR_String 1
    #define DESTRUCTOR_String DestroyString
end_class(String)

static void DestroyString(String *self) {
    free(self->data_);
    self->data_ = NULL;
}
```

### How Destructors Work

**Heap-allocated objects** must be freed manually using `free_class` instead of `free`:

```C
free_class(String, str_ptr)
```

If `CUSTOM_DESTRUCTOR_String` is `0`, this simply calls `free(str_ptr)`. If it is `1`, it first calls your custom destructor and then frees the pointer.

**Stack-allocated objects** are handled automatically. The framework uses the GCC/Clang `__attribute__((cleanup(...)))` extension to call the destructor when the variable goes out of scope, so you never need to free it manually.

## Inheritance

OOPC supports multiple inheritance. To inherit from a parent class, use `INHERIT(ClassName)` inside the child class body:

```C
INHERIT(ClassName)
```

### Example

<table>
<tr>
<th>Before</th>
<th>After</th>
</tr>
<tr>
<td>

```C
class
    #define PUBLIC_Base \
        int a_;
    PUBLIC_Base
    #define CUSTOM_DESTRUCTOR_Base 0
end_class(Base)

class
    INHERIT(Base)
    #define PUBLIC_Object \
        int b_; \
        int c_;
    PUBLIC_Object
    #define CUSTOM_DESTRUCTOR_Object 0
end_class(Object)
```

</td>
<td>

```C
typedef struct {
    int a_;
} Base;

typedef struct {
    int a_;
    int b_;
    int c_;
} Object;
```

</td>
</tr>
</table>

# Demo

Demos are available in the `demo` folder.