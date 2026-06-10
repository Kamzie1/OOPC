#include "utils.h"
#include "class.h"
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

int main() {
    test_container_of();
}
