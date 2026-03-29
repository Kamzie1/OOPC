#include "../src/class.h"
#include "../src/utils.h"
#include <assert.h>

class
    #define PUBLIC_Base\
        int a;
    PUBLIC_Base
end_class(Base)

class
    INHERIT(Base)
    #define PUBLIC_Object\
        int b;\
        int c;
    PUBLIC_Object
end_class(Object)

void test_class_variables_init(){
    Object object;
    int *a_ptr = &object.a;
    Object *container = container_of(a_ptr, Object, a);
    assert(&object == container);
}

int main(){
    test_class_variables_init();
}
