#ifndef PROJECT_GENCODE_H
#define PROJECT_GENCODE_H

#include "genlang/complier/token.h"
#include "genlang/complier/parser.h"

namespace GenLang {

    class generator {
        std::vector<node *> func_decls;
    public:
        void gen(const root_ptr <node> &root);

        void show(const root_ptr <node> &root);
    };


    void generator::show(const root_ptr <node> &root) {
        if (!root)
            return;
        if (is_keyword(root->get("type")->as<String>()->get_val())
            || is_operator(root->get("type")->as<String>()->get_val())
            || root->get("type")->as<String>()->get_val() == "IDENTIFIER"
            || root->get("type")->as<String>()->get_val() == "TYPENAME") {

            if (root->get("val")->as<String>()->get_val() == "{"
                || root->get("val")->as<String>()->get_val() == "}")
                std::cout << std::endl;

            std::cout << root->get("val")->as<String>()->get_val() << " ";
            if (root->get("val")->as<String>()->get_val() == ";"
                || root->get("val")->as<String>()->get_val() == "}"
                || root->get("val")->as<String>()->get_val() == "{")
                std::cout << std::endl;

            return;
        } else if (root->get("val")) {
            std::cout << root->get("val")->to_string() << " ";
            return;
        } else if (root->get("type")->as<String>()->get_val() == "let_stmt") {
            //std::cerr << root->get("matched")->to_string() << std::endl;
            std::cout << "auto ";
            list *a_let = root->get("matched")->as<list>()->get(1) // assigns
                    ->as<node>()->get("matched")->as<list>()->get(0) // assign 0
                    ->as<node>()->get("matched")->as<list>();
            show(a_let->get(0));
            std::cout << " = ";
            show(a_let->get(2));
            std::cout << ";" << std::endl;
            // like : let a = 5;
        } else {
            for (object *e : *root->get("matched")->as<list>()) {
                show(e);
            }

        }
    }

    void generator::gen(const root_ptr <node> &root) {
        std::cout << "#include <cstdio>" << std::endl;
        for (object *o : *root->get("matched")->as<list>()) {
            node *n = o->as<node>();
            if (n->get("type")->as<String>()->get_val() == "func_decl")
                func_decls.push_back(n);
        }
        show(root);
    }
}

#endif //PROJECT_GENCODE_H
