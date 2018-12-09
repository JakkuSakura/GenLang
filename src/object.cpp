#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"

using namespace GenLang;
static void reg()
{
    add_type("object", NULL, typeid(object));
}
static autorun run(reg);
