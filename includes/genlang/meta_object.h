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

namespace GenLang{
struct class_manager;
struct meta_object
{
    meta_object(const char *self, const char *father);
    meta_object *get_father();
    meta_object *get_meta_of_index(int ind);
    const char *name;
    const char *faname;
    std::vector<const char *> memname;
    class_manager *manager;
};
struct class_manager
{
    std::vector<meta_object *> objs;
    void push(meta_object &o);
    meta_object *find(const char *name);
    ~class_manager();
};
}

#endif