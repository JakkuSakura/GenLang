#include "Basic.h"
#include <string>
#include <map>
#include <deque>
#include <queue>
#include <set>
namespace GenLang {

Object *GC::newObject() {
    Object *newobj = new Object();
    objects.insert(newobj);
    return newobj;
}
List *GC::newList() {
    List *newlist = new List();
    objects.insert(newlist);
    return newlist;
}
void GC::autoClean(DynamicType *root) {
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
    for (std::set<DynamicType *>::iterator it = objects.begin(); it != objects.end(); ++it) {
        if(!se.count(*it))
        {
            objects.erase(*it);
            delete *it;
        }
    }
}

GC gc;

}
