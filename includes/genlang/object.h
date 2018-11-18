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

  protected:
    object() {}

  public:
    meta_object *get_type(){return type;}
    virtual string toString() const = 0;
    virtual ~object(){};
};
} // namespace GenLang

#endif
