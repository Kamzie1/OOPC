#include "../src/class.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>

static int print1(){
    printf("1");
}

static int print2(){
    printf("2");
}

class(Object)
    #define FOR_PUBLIC_OF_Object(DECLARE)\
        DECLARE(int, a)\
        DECLARE(int, b)\

    FOR_PUBLIC_OF_Object( DECLARE )

    #define FOR_PRIVATE_OF_Object(DECLARE)\
        DECLARE(int, a_)\
        DECLARE(int, b_)\

    FOR_PRIVATE_OF_Object( DECLARE )

    #define FOR_FUNCTION_OF_Object(REGISTER)\
        REGISTER(void, print1, void)\
        REGISTER(void, print2, void)\

    FOR_FUNCTION_OF_Object( DECLARE_FUNCTIONS )
end_class(Object)

void test_class_variables_init(){
    Object object class_init(Object, object);
    int *a_ptr = &object.a;
    Object *container = container_of(a_ptr, Object, a);
    assert(&object == container);
    CALL_FUNCTION(&object, 0);
}

int main(){
    test_class_variables_init();
}
