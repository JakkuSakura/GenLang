#ifndef GENLANG_GARBAGE_COLLECTION_H
#define GENLANG_GARBAGE_COLLECTION_H

#include "genlang/object.h"
#include "genlang/container.h"
#include <set>


namespace GenLang {
    class grabage_collector {
        std::set<object *> objects;
        std::set<object **> roots;
    public:
        template<class T>
        inline void signin(T *t) {
            objects.insert(t);
        }

        ~grabage_collector() {
            autoClean();
        }

        int autoClean();

        void detach(grabage_collector &gc);

        object *detach(object *dt);

        void attach_root_ptr(object **pPtr);

        void detach_root_ptr(object **pPtr);
    };

    struct class_manager {
        std::vector<meta_object *> objs;
        std::map<void *, meta_object *> types;

        void push(meta_object &o);

        meta_object *find(const char *name);

        meta_object *find(const std::type_info &info);

        ~class_manager();
    };

}
#endif