#ifndef GENLANG_STRING_H
#define GENLANG_STRING_H
#include <string>
#include <cstring>
namespace GenLang{
class string_builder
{
  public:
    std::string s;
    string_builder(const char *cs) : s(cs) {}
    string_builder(const std::string &cs) : s(cs) {}
    string_builder() : s() {}
    string_builder(const string_builder &s2) : s(s2.s) {}
    friend string_builder operator+(const string_builder &a, const string_builder &b)
    {
        return string_builder(a.s + b.s);
    }
    friend bool operator<(const string_builder &a, const string_builder &b)
    {
        return a.s < b.s;
    }
    friend bool operator==(const string_builder &a, const string_builder &b)
    {
        return a.s == b.s;
    }
    string_builder &operator+=(const string_builder &b)
    {
        s += b.s;
        return *this;
    }
    string_builder &operator=(const string_builder &b)
    {
        s = b.s;
        return *this;
    }
    friend void swap(string_builder &a, string_builder &b)
    {
        std::swap(a.s, b.s);
    }
};
typedef const string_builder string;

}
#endif