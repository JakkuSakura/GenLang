#ifndef GENLANG_WRAPPER_H
#define GENLANG_WRAPPER_H
#include "genlang/objects.h"
#include "genlang/string.h"
namespace GenLang{
template <class T>
class wrapper : public object
{
    typedef wrapper<T> thisType;
    T val;

  protected:
    void setVal(const T &v)
    {
        val = v;
    }
    wrapper(T v)
    {
        setVal(v);
    }
    wrapper() : val()
    {
    }

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
    Int()
    {
        setVal(0);
    }

    string toString() const
    {
        char buf[20];
        sprintf(buf, "%d", getVal());
        return buf;
    }
};

class Char : public wrapper<char>
{
  public:
    Char(char ch)
    {
        setVal(ch);
    }
    Char()
    {
        setVal(0);
    }

    string toString() const
    {
        return string("\'") + getVal() + "\'";
    }
};
class Double : public wrapper<double>
{
  public:
    Double(double d)
    {
        setVal(d);
    }
    Double()
    {
        setVal(0.0);
    }
};

class String : public wrapper<string>
{
  public:
    String(const char *s)
    {
        setVal(s);
    }
    String()
    {
        setVal("");
    }

    string toString() const
    {
        return string("\"" + getVal().s + "\"");
    }
};

}
#endif