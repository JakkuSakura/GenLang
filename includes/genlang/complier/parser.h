#ifndef GENLANG_PARSER_H
#define GENLANG_PARSER_H
#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
#include "genlang/complier/token.h"
#include "genlang/complier/lexer.h"

namespace GenLang {
    struct node : public map_object {

        node(token *tk) {
            set_unordered(true);
            put("type", tk->get_token_type());
            put("val", tk->get_val());
        }

        node(const string &type, list *matched_l) {
            set_unordered(true);
            put("type", alloc(String, type));
            put("matched", matched_l);
        }
    };

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
            NORMAL, LEFT, NONE_OR_MORE, ONE_OR_MORE, EMPTY
        };
    }
    class parser {
    public:
        FILE *fout;
        scanner scan;
        parser(FILE *fin, FILE *fout);
        std::multimap<string, item> rule_map;
        std::vector<token *> tokens;


        std::pair<root_ptr<node>, int> match_rule(const string &rule_name, int token_pos);

        void add_rule(const string &name, const string &rule, int type, bool replacable, void (*matched)(parser *p, node *nd) = NULL);
        void scan_src();

        node *parse();

        bool find_typename(const string &val);

    };
    bool is_upper(const string &str);
    bool is_alpha(const string &str);

} // GenLang
#endif