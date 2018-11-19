#ifndef GENLANG_RUNTIME_SUPPORT_H
#define GENLANG_RUNTIME_SUPPORT_H
#include "genlang/object.h"
#include "genlang/meta_object.h"
#include "genlang/garbage_collection.h"
namespace GenLang{
extern class_manager genlang_class_manager;
void add_type(const char *name, const char *fa, const std::type_info &type);

template<class ...T>
object *new_object(const char *type, T... args);
#define alloc(type, args...) (type *)new_object(#type, type, args)
}

#endif // GENLANG_RUNTIME_SUPPORT_H
