#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
namespace GenLang
{
struct node : public map_object
{
    std::vector<node *> matched;
    node(string type)
    {
        put(string("type"), alloc(String, type));
    }

    node(string type, std::vector<node *> &matched_l) {
        matched = matched_l;
        put(string("type"), alloc(String, type));
    }
};
} // GenLang