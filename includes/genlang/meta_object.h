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
    meta_object *get_father();
    meta_object *get_meta_of_index(int ind);
    bool same_as(const std::type_info &info);
    const char *name;
    const char *faname;
    const std::type_info &info;
    std::vector<const char *> memname;
    class_manager *manager;
};
struct class_manager
{
    std::vector<meta_object *> objs;
    void push(meta_object &o);
    meta_object *find(const char *name);
    meta_object *find(const std::type_info &info);
    ~class_manager();
};
}

#endif