#include <bits/stdc++.h>
#include "genlang/complier/lexer.h"

using namespace GenLang;
namespace GenLang
{

static const char *KEYWORDS[] = {
        "let", "as", "class", "struct", "if", "else", "for", "while", "return", "continue", nullptr
};
static const char *TYPENAMES[] = {
        "void", "string", "object", "list", "int", "char", "double", "long", "float", nullptr
};
static const char *OPERATORS[] = {
        ":", ";", "+", "-", "*", "/", "%",
        "**", "<<", ">>", "&&", "&", "||",
        "|", "!", "~", "^", "(", ")", "[", "]",
        "==", "=", "<", ">", "!=", "<=", ">=",
        "+=", "-=", "*=", "/=", "**=", "<<=",
        ">>=", "|=", "&=", "^=", "\"", "\'",
        ".", ",", "?", "//", "{", "}", nullptr
};
    static const char *SPCEIAL_CHARS = R"(~!@#$%^&*()_+-={}[]|\:;"',.<>?/)";

std::set<string> makeset(const char *ks[]) {
    std::set<string> set;
    const char **p = ks;
    while(*p)
        set.insert(*p++);
    return set;
}

scanner::scanner(FILE *fin) : fin(fin)  {
    keywords = makeset(KEYWORDS);
    typenames = makeset(TYPENAMES);
    operators = makeset(OPERATORS);
}

token *scanner::get_token() {
loop:
    string str;
    int ch;
    do ch = getc(fin);
    while(isspace(ch));
    if (ch == EOF)
        return NULL;

    if(isdigit(ch))
    {
        do {
            str += ch;
            ch = getc(fin);
        } while(isdigit(ch));
        if(isalpha(ch) || ch == '_')
            throw "unexpected char";
        ungetc(ch, fin);
        return new_object<token>(alloc(String, "CONSTANT"), alloc(Long, atol(str.get_val().c_str())));
    } else if (isalpha(ch) || ch == '_') {
        do {
            str += ch;
            ch = getc(fin);
        } while(isalpha(ch) || isdigit(ch) || ch == '_');
        ungetc(ch, fin);
        if(keywords.count(str))
            return new_object<token>(alloc(String, str), alloc(String, str));
        else if(typenames.count(str))
            return new_object<token>(alloc(String, "TYPENAME"), alloc(String, str));
        else
            return new_object<token>(alloc(String, "IDENTIFIER"), alloc(String, str));
    } else if(strchr(SPCEIAL_CHARS, ch)) {
        while(operators.count(str + (char)ch)) {
            str += ch;
            ch = getc(fin);
        }
        if (str == "//") {
            while(ch = getc(fin), ch != EOF && ch != '\n')
            ;
            goto loop;
        }
        if(str != "") {
            ungetc(ch, fin);
            return new_object<token>(alloc(String, str), alloc(String, str));
        }
    }
    throw "Unknown token";
}
}
