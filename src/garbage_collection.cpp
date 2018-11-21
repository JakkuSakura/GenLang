#include <string>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <typeinfo>
#include "genlang/garbage_collection.h"
namespace GenLang {
int GC::autoClean(object *root) {
    std::set<object *> vis;
    std::queue<object *> qu;
    qu.push(root);
    vis.insert(root);
    while(!qu.empty())
    {
        object *obj = qu.front();
        qu.pop();
        if(!obj || vis.count(obj)) continue;
        obj->gc_walk(vis, qu);

    }
    
    std::vector<object *> todel;
    int cnt = 0;
    for (auto e : objects) {
        if(!vis.count(e)) {
            todel.push_back(e);
            delete e;
            ++cnt;
        }
    }
    for (auto e : todel) {
        objects.erase(e);
    }
    return cnt;
}

}
