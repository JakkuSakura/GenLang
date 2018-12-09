#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include <string>
using namespace std;
using namespace GenLang;

namespace GenLang
{
class_manager class_manager::inst;
garbage_collector garbage_collector::inst;
void add_type(const char *name, const char *fa, const std::type_info &type)
{
    meta_object *obj = new meta_object(name, fa, type);
    class_manager::inst.push(*obj);
    
}

}
