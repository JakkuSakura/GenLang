#ifndef GENLANG_GARBAGE_COLLECTION_H
#define GENLANG_GARBAGE_COLLECTION_H
#include "genlang/object.h"
#include "genlang/container.h"
#include <set>

namespace GenLang {
class GC {
    std::set<object *> objects;
public:
    template<class T >
    inline void signin(T *t) {
        objects.insert(t);
    }
    template<class T, class... T2>
    inline T *newObj(T2... args) {
        T *n = new T(args...);
        signin(n);
        return n;
    }
    ~GC() {
        autoClean(NULL);
    }
    int autoClean(object *root);
    void detach(GC &gc) {
        gc.objects.insert(objects.begin(), objects.end());
        objects.clear();
    }
    object *detach(object *dt) {
        objects.erase(dt);
        return dt;
    }
};
}
#endif