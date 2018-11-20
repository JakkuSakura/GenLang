#include "genlang/object.h"
#include "genlang/string.h"
#include "genlang/wrapper.h"
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

}
