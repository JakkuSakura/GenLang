#ifndef GENLANG_TOKEN_H
#define GENLANG_TOKEN_H

#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"

namespace GenLang {
    namespace TokenType {
#define tokendef(x) extern String *x;
        tokendef(STRING);
        tokendef(OPERATOR);
        tokendef(KEYWORD);
        tokendef(IDENTIFIER);
        tokendef(TYPENAME);
        tokendef(CONSTANT);
        tokendef(TEOF);
#undef tokendef
    }
    struct token : public map_object {
        token(String *tp, object *v) {
            append("type", tp);
            append("val", v);

        }

        object *get_val() {
            return get("val");
        }

        String *get_token_type() {
            return (String *) get("type");
        }
        bool match(const string &name) {
            return name == get_token_type()->get_val();
        }
    };
}

#endif
