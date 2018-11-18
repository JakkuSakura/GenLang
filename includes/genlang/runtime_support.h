#ifndef GENLANG_RUNTIME_SUPPORT_H
#define GENLANG_RUNTIME_SUPPORT_H
#include "genlang/object.h"
#include "genlang/meta_object.h"
namespace GenLang{
extern class_manager genlang_class_manager;
void add_types(const char *type_list[][2]);
}


#endif // GENLANG_RUNTIME_SUPPORT_H
