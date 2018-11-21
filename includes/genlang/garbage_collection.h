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
    template<class T >
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

}
#endif