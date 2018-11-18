#ifndef GENLANG_BASIC_H
#define GENLANG_BASIC_H
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
    const std::string &getClassName() const {
        return className;
    }
    virtual std::string toString() const = 0;
    virtual ~DynamicType() = default;
};
template<class T>
class BasicType : public DynamicType {
    typedef BasicType<T> thisType;
    T val;
protected:
    void setVal(const T &v) {
        val = v;
    }
    BasicType() = default;

    operator const T () const {
        return getVal();
    }
    operator T () {
        return getVal();
    }
public:
    const T &getVal() const {
        return val;
    }
    T &getVal() {
        return val;
    }
    friend bool operator<(const thisType &a, const thisType &b) {
        return a->getVal() < b->getVal();
    }
    friend bool operator>(const thisType &a, const thisType &b) {
        return a->getVal() > b->getVal();
    }
    friend bool operator==(const thisType &a, const thisType &b) {
        return a->getVal() == b->getVal();
    }
    friend bool operator>=(const thisType &a, const thisType &b) {
        return a->getVal() >= b->getVal();
    }
    friend bool operator<=(const thisType &a, const thisType &b) {
        return a->getVal() <= b->getVal();
    }
};

class Integer : public BasicType<long long> {
public:
    Integer(long long val) {
        setVal(val);
    }
    Integer() {
        setVal(0);
    }

    std::string toString() const override {
        char buf[20];
        sprintf(buf, "%lld", getVal());
        return buf;
    }
};

class Char : public BasicType<char> {
public:
    Char(char ch) {
        setVal(ch);
    }
    Char() {
        setVal(0);
    }

    std::string toString() const override {
        return std::string("\'") + getVal() + "\'";
    }
};
class String : public BasicType<std::string> {
public:
    String(const char *s) {
        setVal(std::string(s));
    }
    String(const std::string &s) {
        setVal(s);
    }
    String() {
        setVal("");
    }

    std::string toString() const override {
        return "\"" + getVal() + "\"";
    }
};
class Double : public BasicType<double> {
public:
    Double(double d) {
        setVal(d);
    }
    Double() {
        setVal(0.0);
    }
};
template<class Mapping>
class Container : public DynamicType {
protected:
    Container() = default;

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
typedef std::pair<std::string, DynamicType *> str_pair;
class Object : public Container
    <std::vector<str_pair>> {
    bool sorted;
    bool unordered;
protected:
    void setUnorded(bool v) {
        unordered = v;
    }
public:
    Object() {
        sorted = true;
        unordered = false;
    }

    int index(const std::string &s) {
        if(unordered) {
            for (int i = 0; i < size(); i++) {
                if(members[i].first == s)
                    return i;
            }
        }else{
            if(!sorted) std::sort(members.begin(), members.end());
            int i = std::lower_bound(members.begin(), members.end(), std::make_pair(s, (DynamicType *)NULL)) - begin();
            if (i < size() && members[i].first == s)
                return i;
        }
        return -1;
    }

    DynamicType *get(const std::string &name) {
        int i = index(name);
        if(i >= 0)
            return members[i].second;
        return NULL;
    }
    const DynamicType *get(const std::string &name) const {
        return ((Object *)this)->get(name);
    }
    template<class T>
    T *put(const std::string &name, T *dt) {
        int i = index(name);
        if(i >= 0)
            members[i].second = dt;
        else
            append(name, dt);
        return dt;
    }

    template<class T>
    T *append(const std::string &name, T *dt) {
        members.push_back(std::make_pair(name, dt));
        sorted = false;
        return dt;
    }
    std::string getMembers() const {
        std::string buf;
        for (const_iterator it = begin(); it != end(); ++it) {
            if(it != begin()) buf += ",";
            buf += "\"";
            buf += it->first;
            buf += "\"";
            buf += ":";
            buf += it->second->toString();
        }
        return buf;
    }

    std::string toString() const override {
        std::string buf = getClassName();
        buf += "{";
        buf += getMembers();
        buf += "}";
        return buf;
    }


};
class List : public Container<std::deque<DynamicType *>> {
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

    std::string toString() const override {
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
