#ifndef GENLANG_SCANNER_H
#define GENLANG_SCANNER_H
#include "genlang/object.h"
#include "genlang/complier/token.h"
namespace GenLang{
    struct scanner {
        FILE *fin;
        scanner(FILE *fin);
        token *get_token();
        std::set<string> keywords;
        std::set<string> typenames;
        std::set<string> operators;

    };
}

#endif
