#ifndef PROJECT_GENCODE_H
#define PROJECT_GENCODE_H

#include "genlang/complier/token.h"
#include "genlang/complier/node.h"
#include "genlang/complier/lexer.h"

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


}

#endif //PROJECT_GENCODE_H
