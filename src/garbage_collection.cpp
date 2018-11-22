#include <string>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <typeinfo>
#include <genlang/garbage_collection.h>
#include <genlang/runtime_support.h>

#include "genlang/garbage_collection.h"

namespace GenLang {
    int grabage_collector::autoClean() {
        std::set<object *> vis;
        std::queue<object *> qu;
        for (auto e : roots) {
            qu.push(*e);
            vis.insert(*e);
        }
        while (!qu.empty()) {
            object *obj = qu.front();
            qu.pop();
            if (!obj || vis.count(obj)) continue;
            obj->gc_walk(vis, qu);

        }

        std::vector<object *> todel;
        int cnt = 0;
        for (auto e : objects) {
            if (!vis.count(e)) {
                todel.push_back(e);
                genlang_class_manager.types.erase(e);
                delete e;
                ++cnt;
            }
        }
        for (auto e : todel) {
            objects.erase(e);
        }
        std::cerr << "deleted " << cnt << "" << std::endl;
        return cnt;
    }


    object *grabage_collector::detach(object *dt) {
        objects.erase(dt);
        return dt;
    }

    void grabage_collector::detach(grabage_collector &gc) {
        gc.objects.insert(objects.begin(), objects.end());
        objects.clear();
    }

    void grabage_collector::attach_root_ptr(object **pPtr) {
        roots.insert(pPtr);
    }

    void grabage_collector::detach_root_ptr(object **pPtr) {
        roots.erase(pPtr);
    }

    void class_manager::push(meta_object &o) {
        objs.push_back(&o);
        o.manager = this;
        std::cerr << "registered " << o.name << " " << o.info.name() << std::endl;
    }

    meta_object *class_manager::find(const char *name) {
        if (!name)
            return NULL;
        for (auto &obj : objs)
            if (std::strcmp(name, obj->name) == 0)
                return obj;
        return NULL;
    }

    meta_object *class_manager::find(const std::type_info &info) {
        for (auto &obj : objs)
            if (obj->info == info)
                return obj;
        return NULL;
    }

    class_manager::~class_manager() {
        for (auto e : objs)
            delete e;
        objs.clear();
    }


}
