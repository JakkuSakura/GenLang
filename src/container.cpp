#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include "genlang/parser.h"

static const char *basic_type_list[][2] = {
    {"container", "object"},
    {"map_object", "container"},
    {"list", "map_object"},
    {0, 0}};
static void reg()
{
    GenLang::add_types(basic_type_list);
}
static autorun run(reg);
