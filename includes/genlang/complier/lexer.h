#ifndef GENLANG_SCANNER_H
#define GENLANG_SCANNER_H

#include "genlang/object.h"
#include "genlang/complier/token.h"

namespace GenLang {
    struct scanner {
        FILE *fin;
        int col = 0, row = 0;
        scanner() {
            fin = stdin;
        }

        scanner(FILE *fin);

        root_ptr<token> get_token();
        int getc(FILE *f);
        void ungetc(int ch, FILE *f);

        std::set<string> keywords;
        std::set<string> typenames;
        std::set<string> operators;

    };

    bool is_keyword(const string &s);

    bool is_operator(const string &s);

}

#endif
