#include "genlang/utils.h"
#include "genlang/complier/parser.h"
#include "genlang/complier/gencode.h"

using namespace GenLang;
using namespace std;
using namespace rule_types;
void reg_typename(parser *p, node *nd)
{
    const GenLang::string &s = nd->get("matched")->as<list>()->get(1)->as<node>()->get("val")->as<String>()->get_val();
    p->scan.typenames.insert(s);
}

void reg_rules(parser &pr) {
    pr.add_rule("parentheses", "( math_expr )", NORMAL, false);
    pr.add_rule("args", "math_expr ,", LEFT, false);
    pr.add_rule("args", "", EMPTY, false);
    pr.add_rule("call", "IDENTIFIER ( args )", NORMAL, false);

    pr.add_rule("list", "[ args ]", NORMAL, false);
    pr.add_rule("obj_pair", "IDENTIFIER : math_expr", NORMAL, false);
    pr.add_rule("obj_pairs", "obj_pair ,", LEFT, false);
    pr.add_rule("obj_pairs", "", EMPTY, false);

    pr.add_rule("map_obj", "{ obj_pairs }", NORMAL, false);


    pr.add_rule("atom", "call", NORMAL, true);
    pr.add_rule("atom", "list", NORMAL, true);
    pr.add_rule("atom", "map_obj", NORMAL, true);
    pr.add_rule("atom", "parentheses", NORMAL, true);
    pr.add_rule("atom", "CONSTANT", NORMAL, true);
    pr.add_rule("atom", "IDENTIFIER", NORMAL, true);
    pr.add_rule("atom", "STRING", NORMAL, true);

    pr.add_rule("power", "atom ** power", NORMAL, true);
    pr.add_rule("power", "atom", NORMAL, true);
    pr.add_rule("multi", "power * / %", LEFT, true);
    pr.add_rule("add", "multi + -", LEFT, true);

    pr.add_rule("compare", "add == != === !== < > <= >=", LEFT, true);

    pr.add_rule("math_expr", "compare", NORMAL, true);

    pr.add_rule("expr", "math_expr", NORMAL, true);


    pr.add_rule("if_stmt", "if ( math_expr ) stmt else stmt", NORMAL, false);
    pr.add_rule("if_stmt", "if ( math_expr ) stmt", NORMAL, false);
    pr.add_rule("while_stmt", "while ( math_expr ) stmt", NORMAL, false);

    pr.add_rule("assign", "IDENTIFIER = math_expr", NORMAL, false);

    pr.add_rule("assign", "IDENTIFIER as TYPENAME", NORMAL, false);

    pr.add_rule("assigns", "assign ,", LEFT, false);

    pr.add_rule("let_stmt", "let assigns ;", NORMAL, false);

    pr.add_rule("let_stmt", "let args as TYPENAME ;", NORMAL, false);


    pr.add_rule("stmt", "expr ;", NORMAL, false);
    pr.add_rule("stmt", ";", NORMAL, false);
    pr.add_rule("stmt", "if_stmt", NORMAL, false);
    pr.add_rule("stmt", "while_stmt", NORMAL, false);
    pr.add_rule("stmt", "stmtblk", NORMAL, false);
    pr.add_rule("stmt", "let_stmt", NORMAL, true);

    pr.add_rule("pair", "TYPENAME IDENTIFIER", NORMAL, false);

    pr.add_rule("decl_args", "pair ,", LEFT, false);
    pr.add_rule("decl_args", "", EMPTY, false);

    pr.add_rule("func_decl", "pair ( decl_args ) ", NORMAL, false);

    pr.add_rule("func_def", "func_decl stmtblk ", NORMAL, false);

    pr.add_rule("class_decl", "class IDENTIFIER", NORMAL, false, reg_typename);
    pr.add_rule("class_def", "class_decl stmtblk", NORMAL, false);

    pr.add_rule("stmt", "class_decl ;", NORMAL, true);

    pr.add_rule("stmt", "class_def", NORMAL, true);

    pr.add_rule("stmt", "func_decl ;", NORMAL, true);
    pr.add_rule("stmt", "func_def", NORMAL, true);

    pr.add_rule("stmtblk", "{ stmts }", NORMAL, false);

    pr.add_rule("stmts", "stmt", NONE_OR_MORE, true);
}

int main() {
    FILE *f = fopen("input.txt", "r");
    parser parser1(f, stdout);

    reg_rules(parser1);


    try {
        parser1.scan_src();
        for (auto e : parser1.tokens)
            cout << e->to_string() << endl;

        root_ptr<node> val = parser1.parse();
        if (val) {
            for (auto e : *(list *) val->get("matched"))
                cout << e->to_string() << endl;
        }
        generator gen;
        gen.gen(val);
    } catch (const char *s) {
        cerr << s << endl;
    } catch (const std::string &s) {
        cerr << s << endl;
    }

    return 0;
}
