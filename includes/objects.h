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
class object {
    std::string className;
protected:
    object() {}
    void setClassName(const std::string &name) {
        className = name;
    }
public:
    const std::string &getClassName() const {
        return className;
    }
    virtual std::string toString() const = 0;
    virtual ~object() = default;
};
template<class T>
class Wrapper : public object {
    typedef Wrapper<T> thisType;
    T val;
protected:
    void setVal(const T &v) {
        val = v;
    }
    Wrapper() = default;

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
};

class Int : public Wrapper<int> {
public:
    Int(int val) {
        setVal(val);
    }
    Int() {
        setVal(0);
    }

    std::string toString() const {
        char buf[20];
        sprintf(buf, "%d", getVal());
        return buf;
    }
};

class Char : public Wrapper<char> {
public:
    Char(char ch) {
        setVal(ch);
    }
    Char() {
        setVal(0);
    }

    std::string toString() const {
        return std::string("\'") + getVal() + "\'";
    }
};
class String : public Wrapper<std::string> {
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

    std::string toString() const {
        return "\"" + getVal() + "\"";
    }
};
class Double : public Wrapper<double> {
public:
    Double(double d) {
        setVal(d);
    }
    Double() {
        setVal(0.0);
    }
};
template<class T>
class contianer : public object {
protected:
    contianer() = default;

public:
    typedef typename T::iterator iterator;
    typedef typename T::const_iterator const_iterator;

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
    T members;
};
typedef std::pair<std::string, object *> str_pair;
class map_object : public contianer
    <std::vector<str_pair> > {
    bool sorted;
    bool unordered;
protected:
    void set_unordered(bool v) {
        unordered = v;
    }
public:
    map_object() {
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
            int i = std::lower_bound(members.begin(), members.end(), std::make_pair(s, (object *)NULL)) - begin();
            if (i < size() && members[i].first == s)
                return i;
        }
        return -1;
    }

    object *get(const std::string &name) {
        int i = index(name);
        if(i >= 0)
            return members[i].second;
        return NULL;
    }
    const object *get(const std::string &name) const {
        return ((map_object *)this)->get(name);
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
    std::string get_members() const {
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

    std::string toString() const {
        std::string buf = getClassName();
        buf += "{";
        buf += get_members();
        buf += "}";
        return buf;
    }


};
class list : public contianer<std::deque<object *>> {
public:
    object *get(int id) {
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

    std::string toString() const {
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
