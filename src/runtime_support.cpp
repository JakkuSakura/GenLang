#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include <string>
using namespace std;
using namespace GenLang;

namespace GenLang
{
class_manager genlang_class_manager;
// GC genlang_garbage_collector;
void add_type(const char *name, const char *fa, const std::type_info &type)
{
    meta_object *obj = new meta_object(name, fa, type);
    genlang_class_manager.push(*obj);
    
}
template<class Tp, class ...T>
object *new_object(const char *type, Tp ty, T... args)
{
    object *obj = new Tp(args...);
    obj->set_type(genlang_class_manager.find(typeid(Tp)));
    return obj;
}
}
