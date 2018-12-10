#ifndef GENLANG_SCANNER_H
#define GENLANG_SCANNER_H

#include "genlang/object.h"
#include "genlang/complier/token.h"

namespace GenLang {
    struct scanner {
        FILE *fin;
        char *file = nullptr;
        int *rows = nullptr, *cols = nullptr;
        int len = 0;
        int col = 0, row = 0;
        int index = 0;

        scanner();

        ~scanner();

        scanner(FILE *fin);

        void readfile();

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
