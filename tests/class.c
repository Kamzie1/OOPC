#include "../src/class.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>

class
    #define PUBLIC_Base \
        int a_;
    PUBLIC_Base

    #define FOR_CONSTRUCTORS_Base(REGISTER)\
        REGISTER(construct_base, int)

    #define CUSTOM_DESTRUCTOR_Base 0
end_class(Base)

static void construct_base(Base *this, int a){
    this->a_ = a;
}

class
    INHERIT(Base)
    #define PUBLIC_Object \
        int b_; \
        int c_;
    PUBLIC_Object

    #define FOR_CONSTRUCTORS_Object(REGISTER)\
        REGISTER(construct_obj)

    #define CUSTOM_DESTRUCTOR_Object 0
end_class(Object)

static void construct_obj(Object* this){};

class
    #define PUBLIC_Abc \
        float a_; \
        int b_; \
        double c_;
    PUBLIC_Abc

    #define FOR_CONSTRUCTORS_Abc(REGISTER)\
        REGISTER(construct_abc0)\
        REGISTER(construct_abc3, float, int, double)\
        REGISTER(construct_abc1_int, int)\
        REGISTER(construct_abc1_float, float)

    #define CUSTOM_DESTRUCTOR_Abc 0
end_class(Abc)

static void construct_abc3(Abc* this, float a, int b, double c){
    this->a_ = a;
    this->b_ = b;
    this->c_ = c;
}

static void construct_abc0(Abc* this){
    this->a_ = 0;
    this->b_ = 0;
    this->c_ = 1;
}

static void construct_abc1_float(Abc* this, float a){
    this->a_ = a;
    this->b_ = 0;
    this->c_ = 0;
}

static void construct_abc1_int(Abc* this, int b){
    this->a_ = 0;
    this->b_ = b;
    this->c_ = 0;
}

void test_constructors() {
    printf("====TEST CLASS CONSTRUCTORS====\n");

    init_class(Base, base, 1)
    assert(base.a_ == 1);

    init_class(Abc, abc, (float)1, 2, (double)3)
    assert(abc.a_ == 1);
    assert(abc.b_ == 2);
    assert(abc.c_ == 3);

    init_class(Abc, abc_empty)
    assert(abc_empty.a_ == 0);
    assert(abc_empty.b_ == 0);
    assert(abc_empty.c_ == 1);

    init_class_ptr(Abc, abc_ptr, (float)1, 2, (double)3)
    assert(abc_ptr->a_ == 1);
    assert(abc_ptr->b_ == 2);
    assert(abc_ptr->c_ == 3);

    init_class_ptr(Abc, abc_ptr_default)
    assert(abc_ptr_default->a_ == 0);
    assert(abc_ptr_default->b_ == 0);
    assert(abc_ptr_default->c_ == 1);

    free(abc_ptr_default);
    free(abc_ptr);
    printf("\n");
}

void test_variable_inheritance() {
    printf("====TEST CLASS VARIABLE INHERITANCE====\n");

    init_class(Object, object)
    int *a_ptr = &object.a_;
    Object *container = container_of(a_ptr, Object, a_);
    assert(&object == container);

    printf("\n");
}

int main() {
    test_variable_inheritance();
    test_constructors();
}
