#ifndef GENLANG_RUNTIME_SUPPORT_H
#define GENLANG_RUNTIME_SUPPORT_H

#include "genlang/object.h"
#include "genlang/meta_object.h"
#include "genlang/garbage_collection.h"

namespace GenLang {

    void add_type(const char *name, const char *fa, const std::type_info &type);

    template<class Tp, class ...T>
    Tp *new_object(const char *name, T... args) {
        Tp *obj = new Tp(args...);
        class_manager::instance().put_type(obj, class_manager::instance().find(name));
        garbage_collector::instance().signin(obj);
        return obj;
    }



    template<class T>
    class root_ptr {
        object *p;
    public:
        root_ptr(T *x = NULL) {
            p = x;
            garbage_collector::instance().attach_root_ptr((object **) this);
        }


        ~root_ptr() {
            garbage_collector::instance().detach_root_ptr((object **) this);
        }

        T &operator*() const {
            return *(T *) p;
        }

        T *operator->() const {
            return (T *) p;
        }

        T *get_p() const {
            return (T *) p;
        }

        operator bool() const {
            return p != NULL;
        }

        operator T *() const {
            return (T *) p;
        }

    };

#define alloc_p(type, ...) root_ptr<type>(new_object<type>(#type, ##__VA_ARGS__))
#define alloc(type, ...) alloc_p(type, ##__VA_ARGS__).get_p()


}

#endif // GENLANG_RUNTIME_SUPPORT_H
