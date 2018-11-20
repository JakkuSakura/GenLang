#include "genlang/utils.h"
#include "genlang/complier/parser.h"
#include <iostream>
using namespace GenLang;
using namespace std;
int main()
{
    FILE *f = fopen("input.txt", "r");
    parser parser1(f, stdout);

    parser1.add_rule("atom", "CONSTANT", 0, true);
    parser1.add_rule("power", "atom OPERATOR#** power", 0, true);
    parser1.add_rule("power", "atom", 0, true);
    parser1.add_rule("multi", "power OPERATOR#* OPERATOR#/ OPERATOR#%", 1, true);
    parser1.add_rule("add", "multi OPERATOR#+ OPERATOR#-", 1, true);

    parser1.add_rule("root", "add", 1, true);
    try {
        parser1.scan_src();
        for(auto e : parser1.tokens)
            cout << e->to_string() << endl;
        cout << parser1.match("root", 0).first->to_string() << endl;
    } catch (const char *s)
    {
        cerr << s << endl;
    }

    return 0;
}