#include <bits/stdc++.h>
enum ClassName { INTEGER, CHAR, string };
struct ScannerVal{
    ClassName type;
    union{
            long long integer;
            char chr;
            string *s;
    };
};
