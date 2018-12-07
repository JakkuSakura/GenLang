#ifndef GENLANG_TOKEN_H
#define GENLANG_TOKEN_H

#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"

namespace GenLang {
    struct token : public map_object {
        token(String *tp, object *v) {
            append("type", tp);
            append("val", v);
        }
        token(String *tp) {
            append("type", tp);
            append("val", tp);
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
        token *set(int col, int row) {
            append("col", alloc(Int, col));
            append("row", alloc(Int, row));
            return this;
        }
    };
}

#endif
