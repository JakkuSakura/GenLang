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
        enum ContainerType{
            NONE, AND, OR, TOKEN
        } mode = NONE;
        enum RepType {
            ONCE, OPTIONAL, ONCE_OR_MORE, ANY, REP
        } rep = ONCE;
        string val, tip;
        int rep_begin, rep_end;
        bool replaceable = false, extract = false, errorat = false;

        int r_begin();

        int r_end();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
        regular_node *get(int i) const
        {
            return (regular_node *)list::get(i);
        }
#pragma clang diagnostic pop

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

        root_ptr<regular_node> new_rp_node(std::stack<root_ptr<regular_node> > &st, regular_node::ContainerType type);

        root_ptr<regular_node> build(const root_ptr<list> &tkls, int pos);

        void add_rule(const string &str);

        void print_all();

        std::pair<root_ptr<list>, int>
        match_node(const root_ptr<regular_node> &root_node, int token_pos);


        std::pair<root_ptr<node>, int> match_rule(const string &rule_name, int token_pos);

        root_ptr<node> parse();

        void put_token(const root_ptr<token> &tk);
    };


}


#endif //GENLANG_REGULAR_PARSER_H
