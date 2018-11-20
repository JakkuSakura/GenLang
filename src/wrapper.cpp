#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/wrapper.h"
#include "genlang/runtime_support.h"
using namespace std;
using namespace GenLang;

namespace GenLang
{
static void reg()
{
    add_type("Int", "object", typeid(Int));
    add_type("Long", "object", typeid(Long));
    add_type("Char", "object", typeid(Char));
    add_type("Double", "object", typeid(Double));
    add_type("String", "object", typeid(String));
    add_type("StringBuilder", "object", typeid(StringBuilder));
}
static autorun run(reg);

}