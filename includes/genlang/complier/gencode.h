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
        if (root->get("val")) {
            if (is_keyword(root->get_type())
                || is_operator(root->get_type())
                || root->get_type() == "IDENTIFIER"
                || root->get_type() == "TYPENAME"
                || root->get_type() == "NATIVE" ) {

                if (root->get_val() == "{"
                    || root->get_val() == "}")
                    std::cout << std::endl;

                std::cout << root->get_val() << " ";
                if (root->get_val() == ";"
                    || root->get_val() == "}"
                    || root->get_val() == "{")
                    std::cout << std::endl;

                return;
            } else {
                std::cout << root->get("val")->to_string() << " ";
                return;
            }

        } else if (root->get_type() == "let_stmt") {

            // like : let a as int;
            list *a_let = root->get_matched();
            show(a_let->get(3));

            show(a_let->get(1));
            std::cout << ";" << std::endl;
        } else {
            for (object *e : *root->get_matched()) {
                show(e);
            }

        }
    }

    void generator::gen(const root_ptr <node> &root) {
        std::cout << "#include <cstdio>" << std::endl;
        for (object *o : *root->get_matched()) {
            node *n = o->as<node>();
            if (n->get_type() == "func_decl")
                func_decls.push_back(n);
        }
        show(root);
    }
}

#endif //PROJECT_GENCODE_H
