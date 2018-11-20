#ifndef GENLANG_TOKEN_H
#define GENLANG_TOKEN_H

#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"

namespace GenLang {
    namespace TokenType {
#define tokentp(nm) String *nm = alloc(String, #nm)
        tokentp(STRING);
        tokentp(OPERATOR);
        tokentp(KEYWORD);
        tokentp(IDENTIFIER);
        tokentp(TYPENAME);
        tokentp(CONSTANT);
        tokentp(TEOF);
    }
    struct token : public map_object {
        token(String *tp, object *v) {
            append("type", tp);
            append("val", v);

        }

        object *getVal() {
            return get("val");
        }

        String *getTokenType() {
            return (String *) get("type");
        }
    };
}

#endif
