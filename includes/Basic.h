#ifndef GENLANG_BASIC
#define GENLANG_BASIC
#include "Basic.h"
#include <algorithm>
#include <cstdlib>
#include <string>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <iostream>
namespace GenLang {
class DynamicType {
    std::string className;
protected:
    DynamicType() {}
    void setClassName(const std::string &name) {
        className = name;
    }
public:
    // TODO create these objects
    enum Type { OBJECT, LIST, INTEGER, CHAR, STRING, DOUBLE } dtype;

    void setType(Type type) {
        dtype = type;
    }
    Type getType() {
        return dtype;
    }
    const std::string &getClassName() const {
        return className;
    }
    virtual std::string toString() const = 0;
    virtual ~DynamicType() {}
};
template<class T, DynamicType::Type ty>
class BasicType : public DynamicType {
    T val;
protected:
    void setVal(const T &v) {
        val = v;
    }
    BasicType() {
        setType(ty);
    }
public:
    const T &getVal() const {
        return val;
    }
};

class Integer : public BasicType<long long, DynamicType::Type::INTEGER> {
public:
    Integer(long long val) {
        setVal(val);
    }
    Integer() {
        setVal(0);
    }
    virtual std::string toString() {
        char buf[20];
        sprintf(buf, "%lld", getVal());
        return buf;
    }
};

class Char : public BasicType<char, DynamicType::Type::CHAR> {
public:
    Char(char ch) {
        setVal(ch);
    }
    Char() {
        setVal(0);
    }
};
class String : public BasicType<std::string, DynamicType::Type::STRING> {
public:
    String(const std::string &s) {
        setVal(s);
    }
    String() {
        setVal("");
    }
    virtual std::string toString() const {
        return "\"" + getVal() + "\"";
    }
};
class Double : public BasicType<double, DynamicType::Type::DOUBLE> {
public:
    Double(double d) {
        setVal(d);
    }
    Double() {
        setVal(0.0);
    }
};
template<class Mapping, DynamicType::Type tp>
class Container : public DynamicType {
protected:
    Container() {
        setType(tp);
    }
public:
    typedef typename Mapping::iterator iterator;
    typedef typename Mapping::const_iterator const_iterator;

    iterator begin() {
        return members.begin();
    }
    iterator end() {
        return members.end();
    }
    const_iterator begin() const {
        return members.begin();
    }
    const_iterator end() const {
        return members.end();
    }
    int size() const {
        return members.size();
    }
protected:
    Mapping members;
};
// template<class T>
// struct KeyComparer {
//     bool operator()(const T &a, const T &b) const {
//         return a.first < b.first;
//     }
// };
typedef std::pair<std::string, DynamicType *> str_pair;
class Object : public Container
    <std::vector<str_pair>,
    DynamicType::Type::OBJECT> {
    bool sorted;
    // static KeyComparer<str_pair> comparer;
public:
    Object() {
        sorted = true;
    }

    int index(const std::string &s) {
        if(!sorted) std::sort(members.begin(), members.end());

        int i = std::lower_bound(members.begin(), members.end(), std::make_pair(s, (DynamicType *)NULL)) - begin();
        if (i < size() && members[i].first == s)
            return i;
        return -1;
    }

    DynamicType *get(const std::string &name) {
        int i = index(name);
        if(i >= 0)
            return members[i].second;
        return NULL;
    }
    template<class T>
    T *put(const std::string &name, T *dt) {
        int i = index(name);
        if(i >= 0)
            members[i].second = dt;
        else
            append(name, dt);
        sorted = false;
        return dt;
    }

    template<class T>
    T *append(const std::string &name, T *dt) {
        members.push_back(std::make_pair(name, dt));
        sorted = false;
        return dt;
    }

    virtual std::string toString() const {
        std::string buf = getClassName();
        buf += "{";
        for (const_iterator it = begin(); it != end(); ++it) {
            if(it != begin()) buf += ",";
            buf += "\"";
            buf += it->first;
            buf += "\"";
            buf += ":";
            buf += it->second->toString();
        }
        buf += "}";
        return buf;
    }


};
class List : public Container<std::deque<DynamicType *>, DynamicType::Type::LIST> {
public:
    DynamicType *get(int id) {
        if(id < 0)
            id = members.size() - 1 - id;
        return members[id];
    }
    template<class T>
    T *put(int id, T *dt)
    {
        members[id] = dt;
        return dt;
    }

    template<class T>
    T *append(T *dt)
    {
        members.push_back(dt);
        return dt;
    }
    virtual std::string toString() const {
        std::string buf = "[";
        for (int i = 0; i < size(); i++) {
            if(i) buf += ",";
            buf += members[i]->toString();
        }
        buf += "]";
        return buf;
    }
};
}


#endif
