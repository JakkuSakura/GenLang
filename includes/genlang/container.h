#ifndef GENLANG_CONTAINER_H
#define GENLANG_CONTAINER_H

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <iostream>
#include "genlang/object.h"
#include "genlang/string.h"

namespace GenLang {

    class container : public object {
    protected:
        container() {};

    public:


    protected:
    };

    class map_object : public container {
        typedef std::pair<string, object *> str_pair;
        typedef std::vector<str_pair> T;

        bool sorted;
        bool unordered;
        T members;

    protected:
        void set_unordered(bool v) {
            unordered = v;
        }

    public:
        typedef T::iterator iterator;
        typedef T::const_iterator const_iterator;

        map_object() {
            sorted = true;
            unordered = false;
        }

        int index(const string &s) {
            if (unordered) {
                for (int i = 0; i < size(); i++) {
                    if (members[i].first == s)
                        return i;
                }
            } else {
                if (!sorted)
                    std::sort(members.begin(), members.end());
                int i = int(
                        std::lower_bound(members.begin(), members.end(), std::make_pair(s, (object *) NULL)) - begin());
                if (i < size() && members[i].first == s)
                    return i;
            }
            return -1;
        }

        object *get(const string &name) {
            int i = index(name);
            if (i >= 0)
                return members[i].second;
            return NULL;
        }

        const object *get(const string &name) const {
            return ((map_object *) this)->get(name);
        }

        template<class T>
        T *put(const string &name, T *dt) {
            int i = index(name);
            if (i >= 0)
                members[i].second = dt;
            else
                append(name, dt);
            return dt;
        }

        template<class T>
        T *append(const string &name, T *dt) {
            members.push_back(std::make_pair(name, dt));
            sorted = false;
            return dt;
        }

        string get_members() const {
            string buf;
            for (auto it = begin(); it != end(); ++it) {
                if (it != begin())
                    buf += ",";
                buf += "\"";
                buf += it->first;
                buf += "\"";
                buf += ":";
                buf += it->second->to_string();
            }
            return buf;
        }

        string to_string() const override {
            string buf;// = getClassName();
            buf += "{";
            buf += get_members();
            buf += "}";
            return buf;
        }

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
            return (int) members.size();
        }

        void gc_walk(std::set<object *> &vis, std::queue<object *> &qu) override {
            for (auto &it : *this) {
                if (it.second && !vis.count(it.second)) {
                    vis.insert(it.second);
                    qu.push(it.second);
                }
            }
        }

    };

    class list : public container {
        typedef std::vector<object *> mem_tp;
        typedef mem_tp::iterator iterator;
        typedef mem_tp::const_iterator const_iterator;
        mem_tp members;
    public:
        const object *get(int id) const {
            if (index(id) > members.size())
                return NULL;
            return members[index(id)];
        }

        object *get(int id) {
            if (index(id) > members.size())
                return NULL;
            return members[index(id)];
        }

        template<class T>
        T *put(int id, object *dt) {
            members[id] = dt;
            return dt;
        }

        template<class T>
        T *append(T *dt) {
            members.push_back(dt);
            return dt;
        }

        string to_string() const override {
            string buf = "[";
            for (int i = 0; i < size(); i++) {
                if (i)
                    buf += ",";
                buf += members[i]->to_string();
            }
            buf += "]";
            return buf;
        }

        int index(int i) const {
            if (i < 0) i = (int) members.size() + i;
            return i;
        }

        void del(int i) {
            members.erase(members.begin() + index(i));
        }


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
            return (int) members.size();
        }

        void gc_walk(std::set<object *> &vis, std::queue<object *> &qu) override {
            for (auto &it : *this) {
                if (!vis.count(it)) {
                    vis.insert(it);
                    qu.push(it);
                }
            }

        }

    };
}
#endif