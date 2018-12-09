#include "genlang/meta_object.h"
using namespace std;
namespace GenLang
{
meta_object::meta_object(const char *self, const char *father, const std::type_info &info) : info(info)
{
    name = self;
    faname = father;
}

bool meta_object::same_as(const std::type_info &info)
{
    return this->info == info;
}
bool meta_object::same_as(meta_object *m)
{
    return m == this;
}

} // namespace GenLang