#include "../src/utils.h"
#include <assert.h>

typedef struct {
  int a;
  int b;
  int c;
} abc;

void test_container() {
  abc object;
  int *a_ptr = &object.a;
  abc *container = container_of(a_ptr, abc, a);
  assert(&object == container);
}

#define variadic(...) VA_LEN(__VA_ARGS__)

void test_VA_LEN(){
    assert(variadic() == 0);
    assert(variadic(1) == 1);
    assert(variadic(1, 2) == 2);
}

int main() {
    test_container();
    test_VA_LEN();
}
