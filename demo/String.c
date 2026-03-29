#include "../src/class.h"
#include <assert.h>
#include <string.h>

#define CONSTRUCTOR_String0(self)       \
    self->data_     = NULL;             \
    self->size_     = 0;                \
    self->capacity_ = 0;

#define CONSTRUCTOR_String1(self, data)                     \
    self->size_     = strlen(data);                         \
    self->capacity_ = self->size_;                          \
    self->data_     = (char *)malloc(self->size_ + 1);      \
    strcpy(self->data_, data);

class
    #define PUBLIC_String \
        char  *data_;     \
        size_t size_;     \
        size_t capacity_;
    PUBLIC_String
    #define DESTRUCTOR_String 1
end_class(String)

static void DestroyString(String *self) {
    free(self->data_);
    self->data_ = NULL;
}

int main() {
    init_class(String, empty)
    assert(empty.data_     == NULL);
    assert(empty.size_     == 0);
    assert(empty.capacity_ == 0);

    init_class(String, hello, "Hello world!")
    assert(hello.data_     != NULL);
    assert(hello.size_     == 12);
    assert(hello.capacity_ == 12);
    assert(strcmp(hello.data_, "Hello world!") == 0);

    init_class_ptr(String, str_ptr, "This is ptr!")
    assert(strcmp(str_ptr->data_, "This is ptr!") == 0);
    assert(str_ptr->size_ == 12);

    free_class(String, str_ptr)
}
