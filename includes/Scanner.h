#ifndef GENLANG_SCANNER_H
#define GENLANG_SCANNER_H
#include "Basic.h"
#include "GC.h"
namespace GenLang{
    struct Token : public Object {
        enum Type { STRING, SINGAL, KEYWORD, IDENTIFIER, TYPENAME, CONSTANT, OPERATOR, TEOF } type;
        template<class T>
        Token(Type tp, String *tpname, T *v) {
            setClassName("Token");
            append("type", tpname);
            append("val", v);
            type = tp;
        }
        Object *getVal() {
            return (Object *)get("val");
        }
    };

    struct Scanner {
        GC &gc;
        FILE *fin;
        Scanner(GC &gc, FILE *fin);
        Token *getToken();
        std::set<std::string> keywords;
        std::set<std::string> typenames;
        std::set<std::string> operators;

    };
}

#endif
