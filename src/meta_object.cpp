#include <bits/stdc++.h>
#include "genlang/meta_object.h"
using namespace std;
namespace GenLang
{
    
meta_object::meta_object(const char *self, const char *father)
{
    name = self;
    faname = father;
    manager = NULL;
}
meta_object *meta_object::get_father()
{
    return manager->find(faname);
}
meta_object *meta_object::get_meta_of_index(int ind)
{
    return manager->find(memname[ind]);
}

void class_manager::push(meta_object &o)
{
    objs.push_back(&o);
    o.manager = this;
    std::cerr << "registered " << o.name << std::endl;
}
meta_object *class_manager::find(const char *name)
{
    if (!name)
        return NULL;
    for (unsigned i = 0; i < objs.size(); ++i)
        if (std::strcmp(name, objs[i]->name) == 0)
            return objs[i];
    return NULL;
}
class_manager::~class_manager()
{
    for (auto e : objs)
        delete e;
    objs.clear();
}

} // namespace GenLang