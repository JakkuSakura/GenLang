#include "genlang/complier/lexer.h"
#include "genlang/complier/regular_parser.h"

using namespace GenLang;

int main() {
    object *o = alloc(object);
    std::cout << "wtf " << class_manager::inst.find_name(o) << std::endl;
    regular_parser pr;
    {
        system("pwd");
        FILE *f = fopen("gramma.txt", "r");
        char buf[1000];
        while (fgets(buf, 1000, f)) {
            string s = buf;
            string result;
            result= s.strip();
            s = result;
            if (s.size() && s[0] != '#')
                pr.add_rule(s);
        }
        fclose(f);
    }

//    pr.print_all();

    {
        FILE *f = fopen("input3.txt", "r");
        scanner scan(f);
        scan.readfile();
        token *tk;
        while ((tk = scan.get_token())) {
            pr.put_token(tk);
//            std::cout << tk->to_string() << std::endl;
        }
        fclose(f);
    }
//    GenLang::show_auto_stack = true;
    try {
        auto result = pr.parse();
        std::cout << result->to_string() << std::endl;
    } catch (const string &s) {
        std::cerr << s << std::endl;
    }

}
