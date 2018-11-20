#ifndef GENLANG_RUNTIME_SUPPORT_H
#define GENLANG_RUNTIME_SUPPORT_H
#include "genlang/object.h"
#include "genlang/meta_object.h"
#include "genlang/garbage_collection.h"
namespace GenLang{
extern class_manager genlang_class_manager;
extern GC genlang_garbage_collector;
void add_type(const char *name, const char *fa, const std::type_info &type);


template<class Tp, class ...T>
Tp *new_object(const char *type, T... args)
{
    Tp *obj = new Tp(args...);
    obj->set_meta_object(genlang_class_manager.find(typeid(Tp)));
    genlang_garbage_collector.signin(obj);
    return obj;
}

#define alloc(type, args...) new_object<type>(#type, ##args)
}

#endif // GENLANG_RUNTIME_SUPPORT_H
