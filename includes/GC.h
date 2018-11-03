#ifndef GENLANG_GC_H
#define GENLANG_GC_H
#include "Basic.h"
namespace GenLang {
class GC {
    std::set<DynamicType *> objects;
public:
    template<class T, class... T2>
    inline T *newDynamicType(T2... args) {
        T *n = new T(args...);
        objects.insert(n);
        return n;
    }

    inline Object *newObject() {
        return newDynamicType<Object>();
    }

    inline List *newList() {
        return newDynamicType<List>();
    }
    template<class... T2>
    inline String *newString(T2... args) {
        return newDynamicType<String>(args...);
    }
    template<class... T2>
    inline Integer *newInteger(T2... args) {
        return newDynamicType<Integer>(args...);
    }
    template<class... T2>
    inline Double *newDouble(T2... args) {
        return newDynamicType<Double>(args...);
    }
    ~GC() {
        autoClean(NULL);
    }
    int autoClean(DynamicType *root);
};
}


#endif
