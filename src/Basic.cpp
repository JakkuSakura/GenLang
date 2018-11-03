#include "Basic.h"
#include "GC.h"
#include <string>
#include <map>
#include <deque>
#include <queue>
#include <set>
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
        if(dt->getType() == DynamicType::OBJECT)
        {
            Object *obj = (Object *)dt;
            for (Object::iterator it = obj->begin(); it != obj->end(); ++it) {
                if(it->second && !se.count(it->second))
                {
                    se.insert(it->second);
                    qu.push(it->second);
                }
            }
        } else {
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
    int cnt = 0;
    for (std::set<DynamicType *>::iterator it = objects.begin(); it != objects.end(); ++it) {
        if(!se.count(*it))
        {
            objects.erase(*it);
            delete *it;
            ++cnt;
        }
    }
    return cnt;
}

GC gc;

}
