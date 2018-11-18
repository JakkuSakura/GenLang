#include <string>
#include <cstring>
namespace GenLang{
class string
{
  public:
    std::string s;
    string(const char *cs) : s(cs) {}
    string(const std::string &cs) : s(cs) {}
    string() : s() {}
    string(const string &s2) { s = s2.s; }
    friend string operator+(const string &a, const string &b)
    {
        return string(a.s + b.s);
    }
    friend bool operator<(const string &a, const string &b)
    {
        return a.s < b.s;
    }
    friend bool operator==(const string &a, const string &b)
    {
        return a.s == b.s;
    }
    string &operator+=(const string &b)
    {
        s += b.s;
        return *this;
    }
    
};

}