#ifndef GENLANG_OBJECTS_H
#define GENLANG_OBJECTS_H
#include "genlang/meta_object.h"
#include "genlang/string.h"
namespace GenLang
{
class object
{
  private:
    meta_object *type;
  public:
    object():type(0) {}
    meta_object *get_meta_object(){return type;}
    void set_meta_object(meta_object *t){type = t;}
    virtual string to_string() const {return "";};
    virtual ~object(){};
};
} // namespace GenLang

#endif
