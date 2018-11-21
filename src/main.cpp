#include "genlang/utils.h"
#include "genlang/complier/parser.h"
#include <iostream>

using namespace GenLang;
using namespace std;
using namespace rule_types;
int main()
{
    FILE *f = fopen("input.txt", "r");
    parser parser1(f, stdout);

    parser1.add_rule("atom", "CONSTANT", NORMAL, true);
    parser1.add_rule("atom", "IDENTIFIER", NORMAL, true);
    parser1.add_rule("power", "atom OPERATOR#** power", NORMAL, true);
    parser1.add_rule("power", "atom", NORMAL, true);
    parser1.add_rule("multi", "power OPERATOR#* OPERATOR#/ OPERATOR#%", LEFT, true);
    parser1.add_rule("add", "multi OPERATOR#+ OPERATOR#-", LEFT, true);

    parser1.add_rule("expr", "add", NORMAL, true);

    parser1.add_rule("stmt", "expr OPERATOR#;", NORMAL, false);

    parser1.add_rule("stmts", "stmt", NONE_OR_MORE, true);

    try {
        parser1.scan_src();
        for(auto e : parser1.tokens)
            cout << e->to_string() << endl;
        auto val = parser1.match_rule("stmts", 0).first;
        if(val)
            cout << val->to_string() << endl;
        else
            cout << val << endl;
    } catch (const char *s)
    {
        cerr << s << endl;
    }

    return 0;
}
