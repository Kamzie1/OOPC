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

int main() { test_container(); }
