#ifndef GENLANG_META_OBJECT_H
#define GENLANG_META_OBJECT_H

#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <iostream>
#include <typeinfo>
namespace GenLang{
struct class_manager;
struct meta_object
{
    meta_object(const char *self, const char *father, const std::type_info &info);
    bool same_as(const std::type_info &info);
    bool same_as(meta_object *m);
    const char *name;
    const char *faname;
    const std::type_info &info;
    std::vector<const char *> memname;
};

}

#endif