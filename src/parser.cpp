#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include "genlang/parser.h"
using namespace GenLang;
static void reg()
{
    add_type("node", "map_object", typeid(node));
}
static autorun run(reg);