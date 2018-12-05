#ifndef GENLANG_RUNTIME_SUPPORT_H
#define GENLANG_RUNTIME_SUPPORT_H

#include "genlang/object.h"
#include "genlang/meta_object.h"
#include "genlang/garbage_collection.h"

namespace GenLang {
    extern class_manager genlang_class_manager;
    extern grabage_collector genlang_garbage_collector;

    void add_type(const char *name, const char *fa, const std::type_info &type);

    template<class Tp, class ...T>
    Tp *new_object(T... args) {
        Tp *obj = new Tp(args...);
        genlang_class_manager.types[obj] = genlang_class_manager.find(typeid(Tp));
        genlang_garbage_collector.signin(obj);
        return obj;
    }



    template<class T>
    class root_ptr {
        object *p;
    public:
        root_ptr(object *x = NULL) {
            p = x;
            genlang_garbage_collector.attach_root_ptr((object **) this);
        }


        ~root_ptr() {
            genlang_garbage_collector.detach_root_ptr((object **) this);
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

#define alloc_r(type, args...) new_object<type>(args)
#define alloc_p(type, args...) root_ptr<type>(alloc_r(type, ##args))
#define alloc(type, args...) alloc_p(type, ##args).get_p()


}

#endif // GENLANG_RUNTIME_SUPPORT_H
