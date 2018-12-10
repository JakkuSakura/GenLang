#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include <string>
using namespace std;
using namespace GenLang;

namespace GenLang
{
void add_type(const char *name, const char *fa, const std::type_info &type)
{
    auto *obj = new meta_object(name, fa, type);
    class_manager::instance().push(*obj);

}

}
