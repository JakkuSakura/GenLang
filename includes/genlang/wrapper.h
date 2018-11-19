#ifndef GENLANG_WRAPPER_H
#define GENLANG_WRAPPER_H
#include "genlang/object.h"
#include "genlang/string.h"
namespace GenLang{
template <class T>
class wrapper : public object
{
    typedef wrapper<T> thisType;
    T val;

  protected:
    wrapper(T v) : val(v){}
    wrapper() : val() {}

    operator const T() const
    {
        return getVal();
    }
    operator T()
    {
        return getVal();
    }

  public:
    const T &getVal() const
    {
        return val;
    }
    T &getVal()
    {
        return val;
    }
};

class Int : public wrapper<int>
{
  public:
    Int(int v) : wrapper(v)
    {

    }
    Int() : wrapper(0)
    {
        
    }

    
    string toString() const
    {
        char buf[20];
        sprintf(buf, "%d", getVal());
        return buf;
    }
};
class Long : public wrapper<long long>
{
  public:
    Long(long long v) : wrapper(v)
    {

    }

    Long() : wrapper(0LL)
    {
        
    }

    string toString() const
    {
        char buf[20];
        sprintf(buf, "%lld", getVal());
        return buf;
    }
};

class Char : public wrapper<char>
{
  public:
    Char(char ch) : wrapper(ch)
    {
        
    }
    Char()
    {
        
    }
};
class Double : public wrapper<double>
{
  public:
    Double(double d) : wrapper(d)
    {
    }
    Double()
    {
        
    }
    string toString() const 
    {
        return "???";
    }
};

class StringBuilder : public wrapper<string_builder>
{
  public:
    StringBuilder(const char *s) : wrapper(s)
    {
        
    }
    StringBuilder()
    {
    }
};
class String : public wrapper<string>
{
  public:
    String(const char *s) : wrapper(s)
    {
    }
    String(const string_builder &sb) : wrapper(sb){}
    String()
    {
    }
};


}
#endif