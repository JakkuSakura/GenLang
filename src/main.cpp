//#include "genlang/utils.h"
//#include "genlang/complier/parser.h"
//#include "genlang/complier/gencode.h"
//
//using namespace GenLang;
//using namespace std;
//using namespace rule_types;
//
//void reg_classname(parser *p, node *nd)
//{
//    const GenLang::string &s = nd->get_matched()->get(1)->as<node>()->get("val")->as<String>()->get_val();
//    p->scan.typenames.insert(s);
//    cerr << "class " << s << endl;
//}
//
//void reg_rules(parser &pr) {
//    pr.add_rule("parentheses", "( math_expr )", NORMAL, false);
//
//
//
//    pr.add_rule("args", "math_expr ,", ONE_AND_SPERATOR, false);
//    pr.add_rule("args", "", EMPTY, false);
//
//    pr.add_rule("tuple", "( args )", NORMAL, false);
//
//    pr.add_rule("tuples", "tuple", ONE_OR_MORE, false);
//
//    pr.add_rule("call", "IDENTIFIER subscripts tuples", ONE_AND_MORE, false);
//    pr.add_rule("call", "parentheses subscripts tuples", ONE_AND_MORE, false);
//
//    pr.add_rule("list", "[ args ]", NORMAL, false);
//
//    pr.add_rule("subscript", "[ math_expr ]", NORMAL, false);
//    pr.add_rule("subscripts", "subscript", ONE_OR_MORE, false);
//
//    pr.add_rule("raw_array", "TYPENAME subscripts", NORMAL, false);
//
//
//    pr.add_rule("obj_pair", "IDENTIFIER : math_expr", NORMAL, false);
//    pr.add_rule("obj_pairs", "obj_pair ,", ONE_AND_SPERATOR, false);
//    pr.add_rule("obj_pairs", "", EMPTY, false);
//
//    pr.add_rule("map_obj", "{ obj_pairs }", NORMAL, false);
//
//    pr.add_rule("atom", "call", NORMAL, true);
//    pr.add_rule("atom", "fetch", NORMAL, true);
//    pr.add_rule("atom", "new_obj", NORMAL, true);
//    pr.add_rule("atom", "list", NORMAL, true);
//    pr.add_rule("atom", "map_obj", NORMAL, true);
//    pr.add_rule("atom", "parentheses", NORMAL, true);
//    pr.add_rule("atom", "CONSTANT", NORMAL, true);
//    pr.add_rule("atom", "IDENTIFIER", NORMAL, true);
//    pr.add_rule("atom", "STRING", NORMAL, true);
//
//    pr.add_rule("power", "atom ** power", NORMAL, true);
//    pr.add_rule("power", "atom", NORMAL, true);
//    pr.add_rule("multi", "power * / %", ONE_AND_SPERATOR, true);
//    pr.add_rule("add_rule", "multi + -", ONE_AND_SPERATOR, true);
//
//    pr.add_rule("compare", "add_rule == != === !== < > <= >=", ONE_AND_SPERATOR, true);
//
//    pr.add_rule("assign", "compare = assign", NORMAL, true);
//    pr.add_rule("assign", "compare", NORMAL, true);
//
//    pr.add_rule("assigns", "assign ,", ONE_AND_SPERATOR, false);
//
//    pr.add_rule("math_expr", "assign", NORMAL, true);
//
//
//    pr.add_rule("expr", "math_expr", NORMAL, true);
//
//
//    pr.add_rule("if_stmt", "if ( math_expr ) stmt else stmt", NORMAL, false);
//    pr.add_rule("if_stmt", "if ( math_expr ) stmt", NORMAL, false);
//    pr.add_rule("while_stmt", "while ( math_expr ) stmt", NORMAL, false);
//
//
//
//    // todo
//    pr.add_rule("let_stmt", "let assigns ;", NORMAL, false);
//
//    pr.add_rule("let_stmt", "let args as TYPENAME ;", NORMAL, false);
//    pr.add_rule("let_stmt", "let args as raw_array ;", NORMAL, false);
//
//
//    pr.add_rule("stmt", "expr ;", NORMAL, false);
//    pr.add_rule("stmt", ";", NORMAL, false);
//    pr.add_rule("stmt", "if_stmt", NORMAL, false);
//    pr.add_rule("stmt", "while_stmt", NORMAL, false);
//    pr.add_rule("stmt", "stmtblk", NORMAL, false);
//    pr.add_rule("stmt", "let_stmt", NORMAL, true);
//
//    pr.add_rule("pair", "TYPENAME IDENTIFIER", NORMAL, false);
//
//    pr.add_rule("func_decl", "pair ( decl_args ) ", NORMAL, false);
//    pr.add_rule("func_def", "func_decl stmtblk ", NORMAL, false);
//
//    pr.add_rule("decl_args", "pair ,", ONE_AND_SPERATOR, false);
//    pr.add_rule("decl_args", "", EMPTY, false);
//
//
//
//    pr.add_rule("class_decl", "class IDENTIFIER", NORMAL, false, reg_classname);
//    pr.add_rule("class_def", "class_decl stmtblk", NORMAL, false);
//
//    pr.add_rule("stmt", "class_decl ;", NORMAL, true);
//
//    pr.add_rule("stmt", "class_def", NORMAL, true);
//
//    pr.add_rule("stmt", "func_decl ;", NORMAL, true);
//    pr.add_rule("stmt", "func_def", NORMAL, true);
//    pr.add_rule("native_blk", "NATIVE", NORMAL, true);
//
//    pr.add_rule("stmt", "native_blk", NORMAL, true);
//
//    pr.add_rule("stmtblk", "{ stmts }", NORMAL, false);
//
//    pr.add_rule("stmts", "stmt", NONE_OR_MORE, false);
//}
//
//int main() {
//    srand(time(0));
//    FILE *f = fopen("input.txt", "r");
//    parser parser1(f);
//
//    reg_rules(parser1);
//
//    try {
//        parser1.scan_src();
//        for (auto e : parser1.tokens)
//            cout << e->to_string() << endl;
//
//        root_ptr<node> val = parser1.parse();
//        if (val) {
////            for (auto e : *(list *) val->get("matched"))
////                cout << e->to_string() << endl;
//            cout << val->to_string() << endl;
//            generator gen;
//            gen.gen(val);
//        }
//    } catch (const char *s) {
//        cerr << s << endl;
//    } catch (const std::string &s) {
//        cerr << s << endl;
//    }
//
//    return 0;
//}
#include "genlang/complier/lexer.h"
#include "genlang/complier/regular_parser.h"
using namespace GenLang;
int main()
{
    regular_parser p;
    p.add_rule("multi : CONSTANT ( '*' CONSTANT ) * ");
    p.build_all();
    p.print_all();

    token *tk;
    FILE *f = fopen("input3.txt", "r");
    scanner scan(f);
    while ((tk = scan.get_token())) {
         p.put_token(tk);
    }

    fclose(f);

    auto result = p.match_rule("multi", 0);
    std::cout << result.first->to_string() << std::endl;
}
