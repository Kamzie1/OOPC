#include "../src/class.h"
#include <assert.h>
#include <string.h>

class
    #define PUBLIC_String \
        char  *data_;     \
        size_t size_;     \
        size_t capacity_;
    PUBLIC_String

    #define FOR_CONSTRUCTORS_String(REGISTER)\
        REGISTER(construct_String0)\
        REGISTER(construct_String1, const char*)\
        REGISTER(construct_String1_str, const String*)

    #define CUSTOM_DESTRUCTOR_String 1
    #define DESTRUCTOR_String DestroyString
end_class(String)

static void construct_String0(String* self){
    self->data_     = NULL;
    self->size_     = 0;
    self->capacity_ = 0;
}

static void construct_String1(String* self, const char* data){
    self->size_     = strlen(data);
    self->capacity_ = self->size_;
    self->data_     = (char *)malloc(self->size_ + 1);
    strcpy(self->data_, data);
}

static void construct_String1_str(String* self, const String* str){
    self->size_     = str->size_;
    self->capacity_ = self->size_;
    self->data_     = (char *)malloc(self->size_ + 1);
    strcpy(self->data_, str->data_);
}

static void DestroyString(String *self) {
    free(self->data_);
    self->data_ = NULL;
}

int main() {
    init_class(String, empty)
    assert(empty.data_     == NULL);
    assert(empty.size_     == 0);
    assert(empty.capacity_ == 0);

    init_class(String, hello, (const char*)"Hello world!")
    assert(hello.data_     != NULL);
    assert(hello.size_     == 12);
    assert(hello.capacity_ == 12);
    assert(strcmp(hello.data_, "Hello world!") == 0);

    init_class(String, from_str, (const String*)&hello)
    assert(from_str.data_     != NULL);
    assert(from_str.size_     == 12);
    assert(from_str.capacity_ == 12);
    assert(strcmp(from_str.data_, "Hello world!") == 0);

    init_class_ptr(String, str_ptr, (const char*)"This is ptr!")
    assert(strcmp(str_ptr->data_, "This is ptr!") == 0);
    assert(str_ptr->size_ == 12);

    free_class(String, str_ptr)
}
