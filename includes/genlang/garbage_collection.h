#ifndef GENLANG_GARBAGE_COLLECTION_H
#define GENLANG_GARBAGE_COLLECTION_H

#include "genlang/object.h"
#include "genlang/container.h"
#include <set>


namespace GenLang {
    class garbage_collector {
        std::set<object *> objects;
        std::set<object **> roots;

        garbage_collector();

        ~garbage_collector();

    public:

        void signin(object *t);

        int auto_clean();

        void detach(garbage_collector &gc);

        object *detach(object *dt);

        void attach_root_ptr(object **pPtr);

        void detach_root_ptr(object **pPtr);


        static garbage_collector &instance();
    };

    class class_manager {
        std::vector<meta_object *> objs;
        std::map<void *, meta_object *> types;

        class_manager();
        ~class_manager();
    public:
        void push(meta_object &o);

        void put_type(void *o, meta_object *meta);

        void erase_type(void *o);

        meta_object *find(const char *name);

        meta_object *find(const std::type_info &info);

        string find_name(void *p);

        static class_manager &instance();
    };

}
#endif