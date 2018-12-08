#ifndef GENLANG_OBJECTS_H
#define GENLANG_OBJECTS_H
#include "genlang/meta_object.h"
#include "genlang/string.h"
namespace GenLang
{
class object
{
  public:
    virtual string to_string() const {return "";};
    virtual void gc_walk(std::set<object *> &vis, std::queue<object *> &qu) {}
    template <class T>
            T *as()
    {
        return (T *)this;
    }
    template <class T>
    const T *as() const
    {
        return (const T *)this;
    }
    virtual ~object(){};
};
} // namespace GenLang

#endif
