#include "../src/class.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>

#define CONSTRUCTOR_Base1(self, new_a)\
    self->a = new_a;

#define CONSTRUCTOR_Object0(self)\

#define CONSTRUCTOR_Abc3(self, a, b, c)\
    self->a_ = a;\
    self->b_ = b;\
    self->c_ = c;

#define CONSTRUCTOR_Abc0(self)\
    self->a_ = 0;\
    self->b_ = 0;\
    self->c_ = 1;


class
    #define PUBLIC_Base\
        int a;
    PUBLIC_Base
    #define DESTRUCTOR_Base 0
end_class(Base)

class
    INHERIT(Base)
    #define PUBLIC_Object\
        int b;\
        int c;
    PUBLIC_Object
    #define DESTRUCTOR_Object 0
end_class(Object)

class
    #define PUBLIC_Abc\
        int a_;\
        int b_;\
        int c_;
    PUBLIC_Abc
    #define DESTRUCTOR_Abc 0
end_class(Abc)



void test_constructors(){
    printf("====TEST CLASS CONSTRUCTORS====\n");
    init_class(Base, base, 1)
    assert(base.a == 1);

    init_class(Abc, abc, 1, 2, 3)
    assert(abc.a_ == 1);
    assert(abc.b_ == 2);
    assert(abc.c_ == 3);

    init_class(Abc, abc_empty)
    assert(abc_empty.a_ == 0);
    assert(abc_empty.b_ == 0);
    assert(abc_empty.c_ == 1);

    init_class_ptr(Abc, abc_ptr, 1, 2, 3)
    assert(abc_ptr->a_ == 1);
    assert(abc_ptr->b_ == 2);
    assert(abc_ptr->c_ == 3);

    init_class_ptr(Abc, abc_ptr_null)
    assert(abc_ptr_null->a_ == 0);
    assert(abc_ptr_null->b_ == 0);
    assert(abc_ptr_null->c_ == 1);

    free(abc_ptr_null);
    free(abc_ptr);
    printf("\n");
}

void test_variables_inheritance(){
    printf("====TEST CLASS VARIABLE INHERITANCE====\n");
    init_class(Object, object)
    int *a_ptr = &object.a;
    Object *container = container_of(a_ptr, Object, a);
    assert(&object == container);

    printf("\n");
}

int main(){
    test_variables_inheritance();
    test_constructors();
}
