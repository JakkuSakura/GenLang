#ifndef GENLANG_TOKEN_H
#define GENLANG_TOKEN_H
#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"

namespace GenLang{
    struct token : public map_object {
        enum Type { STRING, SINGAL, KEYWORD, IDENTIFIER, TYPENAME, CONSTANT, OPERATOR, TEOF } type;
        
        token(Type tp, object *v) {
            append("val", v);
            type = tp;
        }
        object *getVal() {
            return get("val");
        }
        Type getTokenType() {
            return type;
        }
    };
}

#endif
