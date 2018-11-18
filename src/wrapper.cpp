#include "genlang/object.h"
#include "genlang/autorun.h"
#include <string>
#include "genlang/runtime_support.h"
using namespace std;
using namespace GenLang;

namespace GenLang
{

static const char *basic_type_list[][2] = {
    {"wrapper", "object"},
    {"Int", "wrapper"},
    {"Char", "wrapper"},
    {"String", "wrapper"},
    {"Double", "wrapper"},
    {0, 0}};
static void reg()
{
    add_types(basic_type_list);
}
static autorun run(reg);

}