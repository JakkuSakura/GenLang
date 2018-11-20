#include "genlang/utils.h"
#include "genlang/complier/parser.h"
#include <iostream>
using namespace GenLang;
using namespace std;
int main()
{
    parser parser1(stdin, stdout);
    parser1.add_rule("add", "CONSTANT OPERATOR#+ CONSTANT", true);
    parser1.scan_src();
    cout << parser1.match("add", 0).first << endl;
    return 0;
}