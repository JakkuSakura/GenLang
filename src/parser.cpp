#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include "genlang/parser.h"
using namespace GenLang;
static void reg()
{
    genlang_class_manager.push(*new meta_object("node", "map_object"));
}
static autorun run(reg);