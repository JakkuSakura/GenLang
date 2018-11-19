#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
namespace GenLang
{
struct node : public map_object
{
    node(string type)
    {
        put(string("type"), alloc(String, type));
    }
};
} // GenLang