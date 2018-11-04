#include <bits/stdc++.h>
#include "Basic.h"
#include "GC.h"
#include "Scanner.h"
using namespace GenLang;
static const char *KEYWORDS[] = {
    "let", "as", "class", "struct", "if", "for", "while", "return", 0
};
static const char *TYPENAMES[] = {
    "void", "string", "object", "list", "int", "char", "double", "long", "float", 0
};
static const char *OPERATORS[] = {
    ":", ";", "+", "-", "*", "/", "%",
    "**", "<<", ">>", "&&", "&", "||",
    "|", "!", "~", "^", "(", ")", "[", "]",
    "==", "=", "<", ">", "!=", "<=", ">=",
    "+=", "-=", "*=", "/=", "**=", "<<=",
    ">>=", "|=", "&=", "^=", "\"", "\'",
    ".", ",", "?", "//", 0
};
static const char *SPCEIAL_CHARS = "~!@#$%^&*()_+-={}[]|\\:;\"\',.<>?/";
#define new(type, args...) gc.newDynamicType<type>(args)
std::set<std::string> makeset(const char *ks[]) {
    std::set<std::string> set;
    const char **p = ks;
    while(*p)
        set.insert(*p++);
    return set;
}

Scanner::Scanner(GC &gc, FILE *fin) : gc(gc), fin(fin)  {
    keywords = makeset(KEYWORDS);
    typenames = makeset(TYPENAMES);
    operators = makeset(OPERATORS);
}

Token *Scanner::getToken() {
loop:
    std::string str;
    int ch;
    do ch = getc(fin);
    while(isspace(ch));
    if (ch == EOF)
        return new(Token, Token::Type::TEOF, new(String, "TEOF"), new(Object));

    if(isdigit(ch))
    {
        do {
            str += ch;
            ch = getc(fin);
        } while(isdigit(ch));
        if(!isspace(ch))
            throw "unexpected char";
        ungetc(ch, fin);
        return new(Token, Token::Type::CONSTANT, new(String, "CONSTANT"), new(Integer, atol(str.c_str())));
    } else if (isalpha(ch) || ch == '_') {
        do {
            str += ch;
            ch = getc(fin);
        } while(isalpha(ch) || isdigit(ch) || ch == '_');
        ungetc(ch, fin);
        if(keywords.count(str))
            return new(Token, Token::Type::KEYWORD, new(String, "KEYWORD"), new(String, str));
        else if(typenames.count(str))
            return new(Token, Token::Type::TYPENAME, new(String, "TYPENAME"), new(String, str));
        else
            return new(Token, Token::Type::IDENTIFIER, new(String, "IDENTIFIER"), new(String, str));
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
            return new(Token, Token::Type::OPERATOR, new(String, "OPERATOR"), new(String, str));
        }
    }
    throw "Unknown token";
}
