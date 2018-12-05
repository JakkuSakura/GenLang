#ifndef GENLANG_PARSER_H
#define GENLANG_PARSER_H
#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
#include "genlang/complier/token.h"
#include "genlang/complier/lexer.h"
#include "genlang/complier/node.h"

namespace GenLang {
    class parser;

    struct item {
        string name;
        list *rule;
        int type;
        bool replacable;
        void (*matched)(parser *p, node *nd);
    };

    namespace rule_types
    {
        enum {
            NORMAL, ONE_AND_SPERATOR, NONE_OR_MORE, ONE_OR_MORE, EMPTY, ONE_AND_MORE
        };
    }
    class parser {
    public:
        scanner scan;
        parser(FILE *fin);
        std::multimap<string, item> rule_map;
        std::vector<token *> tokens;
        std::set<string> typenames;
        std::vector<GenLang::string> st;
        std::pair<root_ptr<node>, int> match_rule(const string &rule_name, int token_pos);

        void add_rule(const string &name, const string &rule, int type, bool replacable, void (*matched)(parser *p, node *nd) = NULL);
        void scan_src();

        node *parse();

        bool find_typename(const string &val);
    };

} // GenLang
#endif