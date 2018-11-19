#include "genlang/object.h"
#include "genlang/autorun.h"
#include <string>
#include "genlang/wrapper.h"
#include "genlang/runtime_support.h"
using namespace std;
using namespace GenLang;

namespace GenLang
{
static void reg()
{
    add_type("wrapper", "object", typeid(object));
    add_type("Int", "wrapper", typeid(Int));
    add_type("Long", "wrapper", typeid(Long));
    add_type("Char", "wrapper", typeid(Char));
    add_type("Double", "wrapper", typeid(Double));
    add_type("String", "wrapper", typeid(String));
    add_type("StringBuilder", "wrapper", typeid(StringBuilder));
}
static autorun run(reg);

}