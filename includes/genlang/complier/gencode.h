#ifndef PROJECT_GENCODE_H
#define PROJECT_GENCODE_H

#include "genlang/complier/token.h"

namespace GenLang {

    class generator {
        std::vector<node *> func_decls;
        std::set<string> classes;
        scanner scan;
    public:
        void gen(const root_ptr <node> &root);

        void show(const root_ptr <node> &root);

        string to_str(const root_ptr <node> &root);
    };

    string generator::to_str(const root_ptr <node> &root) {
        string str;
        if (!root)
            return str;
        if (root->get("val")) {
            if (is_keyword(root->get_type())
                || is_operator(root->get_type())
                || root->get_type() == "IDENTIFIER"
                || root->get_type() == "TYPENAME"
                || root->get_type() == "NATIVE") {

                if (root->get_str_val() == "{"
                    || root->get_str_val() == "}")
                    str += '\n';

                str += root->get_str_val(), str += " ";
                if (root->get_str_val() == ";"
                    || root->get_str_val() == "}"
                    || root->get_str_val() == "{")
                    str += '\n';
            } else {
                str += root->get("val")->to_string();
                str += " ";
            }
            return str;

        } else if (root->get_type() == "let_stmt") {

            if (root->has("as")) {

                list *a_let = root->get_matched();

                string ty = a_let->get(3)->as<node>()->get_str_val();
                char buf[100];

                if (classes.count(ty)) {
                    sprintf(buf, "root_ptr<%s> ", ty.get_val().c_str());
                    str += buf;

                } else if (scan.typenames.count(ty)){
                    str += ty;
                } else {

                    string new_tn;
                    // like : let a as int/int[N];
                    sprintf(buf, "type%d ", rand());
                    new_tn = buf;
                    sprintf(buf, "typedef %s %s;\n", ty.get_val().c_str(), new_tn.get_val().c_str());
                    str += buf;
                    str += new_tn;
                }

                str += to_str(a_let->get(1));
                str += ";\n";
            } else {
                // todo
            }
            return str;
        } else if (root->get_type() == "class_decl") {
            classes.insert(root->get_matched()->get(1)->as<node>()->get_str_val());
        } else if (root->get_type() == "class_def") {
            for (object *e : *root->get_matched()) {
                str += to_str(e);
            }
            str.get_val().pop_back();
            str += ";\n";
            return str;
        } else if (root->get_type() == "call") {

            string name = root->get_matched()->get(0)->as<node>()->get_str_val();
            if (classes.count(name)) {
                str += "alloc(";
                str += name;
                list *tp = root->get_matched()->get(1)->as<node>()->get_matched()->get(0)->as<node>()->get_matched();
                if (tp->size() > 2) {
                    str += ", ";
                    for (int i = 1; i < tp->size() - 1; ++i) {
                        str += to_str(tp->get(i));
                    }
                }
                str += ") ";
                return str;

            }
        }

        for (object *e : *root->get_matched()) {
            str += to_str(e);
        }

        return str;
    }

    void generator::show(const root_ptr <node> &root) {
        std::cout << to_str(root) << std::endl;
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
