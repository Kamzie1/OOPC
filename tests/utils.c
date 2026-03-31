#include "../src/utils.h"
#include "../src/class.h"
#include <assert.h>

typedef struct {
    int a;
    int b;
    int c;
} Abc;

void test_container_of() {
    Abc object;
    int *a_ptr = &object.a;
    Abc *container = container_of(a_ptr, Abc, a);
    assert(&object == container);
}

#define FOR_CONSTRUCTORS_CLASS(REGISTER)\
    REGISTER(add, int, int, int)

void construct_function_type(){
    int a, b, c;
    DISPATCH_CONSTRUCTOR(CLASS, NAME, a, b, c)
}

int main() {
    test_container_of();
    test_va_count();
}
