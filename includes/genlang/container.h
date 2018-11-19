#ifndef GENLANG_CONTAINER_H
#define GENLANG_CONTAINER_H
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <iostream>
#include "genlang/object.h"

namespace GenLang
{

class container : public object
{
  protected:
    container(){};

  public:


  protected:
};
class map_object : public container
{
    typedef std::pair<string, object *> str_pair;
    typedef std::vector<str_pair> T;

    bool sorted;
    bool unordered;
    T members;

  protected:
    void set_unordered(bool v)
    {
        unordered = v;
    }

  public:
    typedef T::iterator iterator;
    typedef T::const_iterator const_iterator;
    map_object()
    {
        sorted = true;
        unordered = false;
    }

    int index(const string &s)
    {
        if (unordered)
        {
            for (int i = 0; i < size(); i++)
            {
                if (members[i].first == s)
                    return i;
            }
        }
        else
        {
            if (!sorted)
                std::sort(members.begin(), members.end());
            int i = std::lower_bound(members.begin(), members.end(), std::make_pair(s, (object *)NULL)) - begin();
            if (i < size() && members[i].first == s)
                return i;
        }
        return -1;
    }

    object *get(const string &name)
    {
        int i = index(name);
        if (i >= 0)
            return members[i].second;
        return NULL;
    }
    const object *get(const string &name) const
    {
        return ((map_object *)this)->get(name);
    }
    template <class T>
    T *put(const string &name, T *dt)
    {
        int i = index(name);
        if (i >= 0)
            members[i].second = dt;
        else
            append(name, dt);
        return dt;
    }

    template <class T>
    T *append(const string &name, T *dt)
    {
        members.push_back(std::make_pair(name, dt));
        sorted = false;
        return dt;
    }
    string get_members() const
    {
        string_builder buf;
        for (const_iterator it = begin(); it != end(); ++it)
        {
            if (it != begin())
                buf += ",";
            buf += "\"";
            buf += it->first;
            buf += "\"";
            buf += ":";
            buf += it->second->toString();
        }
        return buf;
    }

    string toString() const
    {
        string_builder buf;// = getClassName();
        buf += "{";
        buf += get_members();
        buf += "}";
        return buf;
    }
    iterator begin()
    {
        return members.begin();
    }
    iterator end()
    {
        return members.end();
    }
    const_iterator begin() const
    {
        return members.begin();
    }
    const_iterator end() const
    {
        return members.end();
    }
    int size() const
    {
        return members.size();
    }
    
};

class list : public container
{
    typedef std::deque<object *> T;
    T members;
  public:
    object *get(int id)
    {
        if (id < 0)
            id = members.size() - 1 - id;
        return members[id];
    }
    template <class T>
    T *put(int id, T *dt)
    {
        members[id] = dt;
        return dt;
    }

    template <class T>
    T *append(T *dt)
    {
        members.push_back(dt);
        return dt;
    }

    string toString() const
    {
        string_builder buf = "[";
        for (int i = 0; i < size(); i++)
        {
            if (i)
                buf += ",";
            buf += members[i]->toString();
        }
        buf += "]";
        return buf;
    }

    typedef T::iterator iterator;
    typedef T::const_iterator const_iterator;
    iterator begin()
    {
        return members.begin();
    }
    iterator end()
    {
        return members.end();
    }
    const_iterator begin() const
    {
        return members.begin();
    }
    const_iterator end() const
    {
        return members.end();
    }
    int size() const
    {
        return members.size();
    }
};
}
#endif