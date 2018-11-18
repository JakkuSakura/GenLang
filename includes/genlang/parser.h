#include "genlang/object.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
namespace GenLang
{
struct node : public map_object
{
    node(const std::string &type)
    {
    }
};
} // GenLang