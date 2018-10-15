#include "utils.h"
#include <cstring>
#include <cctype>
long long S2LL(const char *s, int base)
{
  long long r = 0;
  while(*s)
  {
    if(isdigit(*s))
      r = r * base + *s - '0';
    else if(*s >= 'A' && *s <= 'Z')
      r = r * base + *s - 'A' + 10;
    else//'a'<=s<='z'
      r = r * base + *s - 'a' + 10;
    ++s;
  }
  return r;
}
