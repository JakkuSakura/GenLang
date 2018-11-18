#include "Basic.h"
#include "GC.h"
#include <string>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <typeinfo>
namespace GenLang {
int GC::autoClean(DynamicType *root) {
    std::set<DynamicType *> se;
    std::queue<DynamicType *> qu;
    qu.push(root);
    se.insert(root);
    while(!qu.empty())
    {
        DynamicType *dt = qu.front();
        qu.pop();
        if(!dt) continue;
        if(typeid(*dt) == typeid(Object))
        {
            Object *obj = (Object *)dt;
            for (Object::iterator it = obj->begin(); it != obj->end(); ++it) {
                if(it->second && !se.count(it->second))
                {
                    se.insert(it->second);
                    qu.push(it->second);
                }
            }
        } else if(typeid(*dt) == typeid(List)) {
            List *lst = (List *)dt;
            for (List::iterator it = lst->begin(); it != lst->end(); ++it) {
                if(!se.count(*it))
                {
                    se.insert(*it);
                    qu.push(*it);
                }
            }
        }
    }
    
    std::vector<DynamicType *> todel;
    int cnt = 0;
    for (auto e : objects) {
        if(!se.count(e)) {
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
