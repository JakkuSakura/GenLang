#ifndef GENLANG_BASIC
#define GENLANG_BASIC
#include "Basic.h"
#include <string>
#include <map>
#include <deque>
#include <queue>
#include <set>
namespace GenLang {
class DynamicType {
    std::string className;
protected:
    DynamicType() {}
    void setClassName(const std::string &name) {
        className = name;
    }
public:
    enum Type { OBJECT, LIST } dtype;

    void setType(Type type) {
        dtype = type;
    }
    Type getType() {
        return dtype;
    }
    const std::string &getClassName() const {
        return className;
    }
    virtual std::string toString() {
        return "";
    }
    virtual ~DynamicType() {}
};
class Object : public DynamicType {
public:
    typedef std::map<std::string, DynamicType *> Mapping;
    typedef Mapping::iterator iterator;
    DynamicType *get(const std::string &name)
    {
        return members[name];
    }
    int size() const
    {
        return members.size();
    }
    virtual const std::string &toString() const {
        return getClassName();
    }
    iterator begin()
    {
        return members.begin();
    }
    iterator end()
    {
        return members.end();
    }
    Object() {
        setType(OBJECT);
    }
private:
    Mapping members;

};

class List : public DynamicType {
public:
    typedef std::deque<DynamicType *> Listing;
    typedef Listing::iterator iterator;
    DynamicType *get(int id) {
        if(id < 0)
            id = members.size() - 1 - id;
        return members[id];
    }
    virtual std::string toString() const
    {
        std::string buf = "[";
        for (int i = 0; i < size(); i++) {
            if(i) buf += ",";
            buf += members[i]->toString();
        }
        buf += "]";
        return buf;
    }
    int size() const{
        return members.size();
    }
    iterator begin()
    {
        return members.begin();
    }
    iterator end()
    {
        return members.end();
    }
    List() {
        setType(LIST);
    }
private:
    Listing members;
};
class GC {
    std::set<DynamicType *> objects;
public:
    Object *newObject();
    List *newList();
    void autoClean(DynamicType *root);
};
extern GC gc;

}


#endif
