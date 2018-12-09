#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"

using namespace GenLang;
static void reg()
{
    add_type("container", "object", typeid(object));
    add_type("map_object", "container", typeid(container));
    add_type("list", "map_object", typeid(map_object));
}
static autorun run(reg);
