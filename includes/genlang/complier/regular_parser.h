//
// Created by jack on 12/5/18.
//

#ifndef GENLANG_REGULAR_PARSER_H
#define GENLANG_REGULAR_PARSER_H

#include <stack>
#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
#include "genlang/complier/token.h"
#include "genlang/complier/node.h"
#include "genlang/utils.h"

namespace GenLang {
    struct regular_node : list {
        enum {
            NONE, AND, OR, ONE_OR_NONE, ONE_OR_MORE, NONE_OR_MORE, NODE, REP
        } mode;
        string name;
        int rep_begin, rep_end;
        bool replaceable, errorat;

        int r_begin();

        int r_end();

        string to_string() const override;

    };

    static const string EMPTY = "EMPTY", EPS = "EPS";

    class regular_parser {
        std::vector<string> input;
        std::multimap<string, root_ptr<regular_node> > rule_map;
        std::vector<root_ptr<token> > tokens;
        std::vector<GenLang::string> st, st2;
    public:
        root_ptr<token> get_token(int i);

        root_ptr<regular_node> add(std::stack<root_ptr<regular_node> > &st);

        root_ptr<regular_node> build(const root_ptr<list> &tkls, int pos);

        void add_rule(const string &str);

        void build_all();

        void print_all();

        std::pair<root_ptr<list>, int>
        match_node(const root_ptr<regular_node> &root_node, int token_pos, bool fatal);


        std::pair<root_ptr<node>, int> match_rule(const string &rule_name, int token_pos, bool fatal);

        void put_token(const root_ptr<token> &tk);
    };


}


#endif //GENLANG_REGULAR_PARSER_H
