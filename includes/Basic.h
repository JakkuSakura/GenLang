#ifndef GENLANG_BASIC
#define GENLANG_BASIC
#include <string>
#include <map>
#include <deque>

namespace GenLang {
class DynamicType {
    std::string className;
protected:
    void setClassName(const std::string &name);
public:
    const std::string &getClassName() const;
};

class Object : public DynamicType {
    std::map<std::string, DynamicType *> members;
    DynamicType *get(const std::string &name);
};

class List : public DynamicType {
    std::deque<DynamicType *> members;
    DynamicType *get(int id);
};

DynamicType *newObject();
}


#endif
