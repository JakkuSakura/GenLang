#include "genlang/utils.h"
#include "genlang/complier/parser.h"
#include <iostream>

using namespace GenLang;
using namespace std;
using namespace rule_types;

int main() {
    FILE *f = fopen("input.txt", "r");
    parser parser1(f, stdout);

    parser1.add_rule("parentheses", "( math_expr )", NORMAL, false);
    parser1.add_rule("args", "math_expr ,", LEFT, false);
    parser1.add_rule("call", "IDENTIFIER ( args )", NORMAL, false);


    parser1.add_rule("atom", "call", NORMAL, true);
    parser1.add_rule("atom", "parentheses", NORMAL, true);
    parser1.add_rule("atom", "CONSTANT", NORMAL, true);
    parser1.add_rule("atom", "IDENTIFIER", NORMAL, true);
    parser1.add_rule("power", "atom ** power", NORMAL, true);
    parser1.add_rule("power", "atom", NORMAL, true);
    parser1.add_rule("multi", "power * / %", LEFT, true);
    parser1.add_rule("add", "multi + -", LEFT, true);


    parser1.add_rule("compare_oper", "==", NORMAL, true);
    parser1.add_rule("compare_oper", "<", NORMAL, true);
    parser1.add_rule("compare_oper", ">", NORMAL, true);
    parser1.add_rule("compare_oper", "!=", NORMAL, true);
    parser1.add_rule("compare_oper", "<=", NORMAL, true);
    parser1.add_rule("compare_oper", ">=", NORMAL, true);
    parser1.add_rule("compare_oper", "===", NORMAL, true);

    parser1.add_rule("compare", "add compare_oper", LEFT, true);

    parser1.add_rule("math_expr", "compare", NORMAL, true);

    parser1.add_rule("expr", "math_expr", NORMAL, true);


    parser1.add_rule("if_stmt", "if ( math_expr ) stmt else stmt", NORMAL, false);
    parser1.add_rule("if_stmt", "if ( math_expr ) stmt", NORMAL, false);
    parser1.add_rule("while_stmt", "while ( math_expr ) stmt", NORMAL, false);
    parser1.add_rule("let_stmt", "let IDENTIFIER = math_expr ;", NORMAL, false);


    parser1.add_rule("stmt", "expr ;", NORMAL, false);
    parser1.add_rule("stmt", ";", NORMAL, false);
    parser1.add_rule("stmt", "if_stmt", NORMAL, false);
    parser1.add_rule("stmt", "while_stmt", NORMAL, false);
    parser1.add_rule("stmt", "stmtblk", NORMAL, false);
    parser1.add_rule("stmt", "let_stmt", NORMAL, true);

    parser1.add_rule("pair", "TYPENAME IDENTIFIER", NORMAL, false);

    parser1.add_rule("decl_args", "pair ,", LEFT, false);

    parser1.add_rule("func_decl", "pair ( decl_args ) ", NORMAL, false);

    parser1.add_rule("func_def", "func_decl stmtblk ", NORMAL, false);


    parser1.add_rule("stmt", "func_decl ;", NORMAL, false);
    parser1.add_rule("stmt", "func_def", NORMAL, false);

    parser1.add_rule("stmtblk", "{ stmts }", NORMAL, false);

    parser1.add_rule("stmts", "stmt", NONE_OR_MORE, true);

    try {
        parser1.scan_src();
//        for (auto e : parser1.tokens)
//            cout << e->to_string() << endl;

        root_ptr<node> val = parser1.parse();
        if (val) {
            for (auto e : *(list *) val->get("matched"))
                cout << e->to_string() << endl;
        }
        parser1.show(val);
    } catch (const char *s) {
        cerr << s << endl;
    } catch (const std::string &s) {
        cerr << s << endl;
    }

    return 0;
}
