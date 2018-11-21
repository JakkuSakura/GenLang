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

    parser1.add_rule("parentheses", "OPERATOR#( math_expr OPERATOR#)", NORMAL, false);
    parser1.add_rule("args", "math_expr OPERATOR#,", LEFT, false);
    parser1.add_rule("call", "IDENTIFIER OPERATOR#( args OPERATOR#)", NORMAL, false);


    parser1.add_rule("atom", "call", NORMAL, true);
    parser1.add_rule("atom", "parentheses", NORMAL, true);
    parser1.add_rule("atom", "CONSTANT", NORMAL, true);
    parser1.add_rule("atom", "IDENTIFIER", NORMAL, true);
    parser1.add_rule("power", "atom OPERATOR#** power", NORMAL, true);
    parser1.add_rule("power", "atom", NORMAL, true);
    parser1.add_rule("multi", "power OPERATOR#* OPERATOR#/ OPERATOR#%", LEFT, true);
    parser1.add_rule("add", "multi OPERATOR#+ OPERATOR#-", LEFT, true);

    parser1.add_rule("let", "KEYWORD#let IDENTIFIER OPERATOR#= math_expr", NORMAL, false);

    parser1.add_rule("compare_oper", "OPERATOR#==", NORMAL, true);
    parser1.add_rule("compare_oper", "OPERATOR#<", NORMAL, true);
    parser1.add_rule("compare_oper", "OPERATOR#>", NORMAL, true);
    parser1.add_rule("compare_oper", "OPERATOR#!=", NORMAL, true);
    parser1.add_rule("compare_oper", "OPERATOR#<=", NORMAL, true);
    parser1.add_rule("compare_oper", "OPERATOR#>=", NORMAL, true);
    parser1.add_rule("compare_oper", "OPERATOR#===", NORMAL, true);

    parser1.add_rule("compare", "add compare_oper", LEFT, true);

    parser1.add_rule("math_expr", "compare", NORMAL, true);

    parser1.add_rule("expr", "math_expr", NORMAL, true);
    parser1.add_rule("expr", "let", NORMAL, true);


    parser1.add_rule("if_stmt", "KEYWORD#if OPERATOR#( math_expr OPERATOR#) stmt KEYWORD#else stmt", NORMAL, false);
    parser1.add_rule("if_stmt", "KEYWORD#if OPERATOR#( math_expr OPERATOR#) stmt", NORMAL, false);
    parser1.add_rule("while_stmt", "KEYWORD#while OPERATOR#( math_expr OPERATOR#) stmt", NORMAL, false);


    parser1.add_rule("stmt", "expr OPERATOR#;", NORMAL, false);
    parser1.add_rule("stmt", ";", NORMAL, false);
    parser1.add_rule("stmt", "if_stmt", NORMAL, false);
    parser1.add_rule("stmt", "while_stmt", NORMAL, false);
    parser1.add_rule("stmt", "stmtblk", NORMAL, false);

    parser1.add_rule("stmtblk", "OPERATOR#{ stmts OPERATOR#}", NORMAL, false);

    parser1.add_rule("stmts", "stmt", NONE_OR_MORE, true);

    try {
        parser1.scan_src();
        for(auto e : parser1.tokens)
            cout << e->to_string() << endl;
        node *val = parser1.match_rule("stmts", 0).first;
        if(val)
        {
            for(auto e : *(list *)val->get("matched"))
                cout << e->to_string() << endl;
        }
    } catch (const char *s)
    {
        cerr << s << endl;
    }

    return 0;
}
