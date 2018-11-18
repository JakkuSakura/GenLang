#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include <string>
using namespace std;
using namespace GenLang;

namespace GenLang
{
class_manager genlang_class_manager;
void add_types(const char *type_list[][2])
{
    auto p1 = type_list;
    while (**p1)
    {
        const char **pair = *p1;
        meta_object *obj = new meta_object(pair[0], pair[1]);
        genlang_class_manager.push(*obj);
        ++p1;
    }
}

}
