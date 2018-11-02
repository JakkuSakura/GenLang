#include "Basic.h"
#include <string>
#include <map>
#include <deque>
namespace GenLang {
class DynamicType {
    enum { OBJECT, LIST } type;
    std::string className;
protected:
    void setClassName(const std::string &name) {
        className = name
    }
public:
    virtual int size();
    virtual const std::string &getClassName() const {
        return className;
    }
    virtual const std::string &toString() const = 0;
};

class Object : public DynamicType {
    typedef std::map<std::string, DynamicType *> Mapping;
    Mapping members;
public:
    DynamicType *get(const std::string &name);
    virtual int size()
    {
        return members.size();
    }
    Object():type(OBJECT) { }

};

class List : public DynamicType {
    std::deque<DynamicType *> members;
public:
    DynamicType *get(int id) {
        if(id < 0)
            id = members.size() - 1 - id;
        return members[id];
    }
    virtual int size()
    {
        return members.size();
    }
    Object():type(LIST) { }
};
class GC {
    std::set<DynamicType *> objects;
    DynamicType *newObject() {
        DynamicType *newobj = new Object();
        object.push_front(newobj);
        return newobj;
    }

    void delObject(DynamicType *dt) {
        objects.erase(dt);
    }
    void autoClean(DynamicType *root) {
        std::set<DynamicType *> se;
        std::queue<DynamicType *> qu;
        qu.push(root);
        while(!qu.empty())
        {
            DynamicType *dt = qu.front();
            qu.pop();
            se.insert(dt);
            if(dt->type == OBJECT)
            {
                Object *obj = (Object *)dt;
                for (Object::Mapping::iterator it = dt.) {
                    /* code */
                }
            } else {

            }
        }
    }
};

}
