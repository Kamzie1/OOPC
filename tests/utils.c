#include "../src/utils.h"
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

void test_va_count() {
    assert(VA_COUNT()     == 0);
    assert(VA_COUNT(1)    == 1);
    assert(VA_COUNT(1, 2) == 2);
}

int main() {
    test_container_of();
    test_va_count();
}