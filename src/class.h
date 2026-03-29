#ifndef CLASS_H
#define CLASS_H
#include "utils.h"
#include <stdarg.h>

#define class typedef struct{

#define end_class(NAME) }NAME;

#define INHERIT(PARENT) PUBLIC_##PARENT

#endif
