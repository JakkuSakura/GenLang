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
        bool replaceable;
        int r_begin() {
            switch (mode) {
                default:
                case NONE:
                case NONE_OR_MORE:
                    return rep_begin = 0;
                case AND:
                case OR:
                case ONE_OR_NONE:
                case ONE_OR_MORE:
                case NODE:
                    return rep_begin = 1;
                case REP:
                    return rep_begin;

            }
        }

        int r_end() {
            switch (mode) {
                default:
                case NONE:
                    return rep_end = 0;

                case AND:
                case OR:
                case NODE:
                case ONE_OR_NONE:
                    return rep_end = 1;

                case ONE_OR_MORE:
                case NONE_OR_MORE:
                    return rep_end = INF;

                case REP:
                    return rep_end;

            }
        }

        string to_string() const override {
            string str;
            if (name.size()) {
                if (name == "*" || name == "?" || name == "*")
                    str += "'";
                str += name;
                if (name == "*" || name == "?" || name == "*")
                    str += "'";
                str += " ";
            } else {
                bool first = true;
                if (size() >= 2)
                    str += "( ";

                for (auto e : *this) {
                    str += e->to_string();
                    if (!first && mode == OR)
                        str += "| ";

                    first = false;
                }
                if (size() >= 2)
                    str += ") ";
            }

            switch (mode) {
                case ONE_OR_MORE:
                    str += "+ ";
                    break;
                case ONE_OR_NONE:
                    str += "? ";
                    break;
                case NONE_OR_MORE:
                    str += "* ";
                    break;
                default:
                    break;
            }
            return str;
        }

    };

    static const string EMPTY = "EMPTY", EPS = "EPS";
    class regular_parser {
        std::vector<string> input;
        std::multimap<string, root_ptr<regular_node> > rule_map;
        std::vector<root_ptr<token> > tokens;
    public:
        root_ptr<token> get_token(int i)
        {
            return tokens[i];
        }
        root_ptr<regular_node> add(std::stack<root_ptr<regular_node> > &st) {
            root_ptr<regular_node> root = alloc(regular_node);
            st.push(root);
            st.top()->mode = regular_node::OR;
            root_ptr<regular_node> r = alloc(regular_node);
            st.top()->append(r.get_p());
            st.push(r);
            st.top()->mode = regular_node::AND;
            return root;
        }

        root_ptr<regular_node> build(const root_ptr<list> &tkls, int pos) {
            std::stack<root_ptr<regular_node> > st;

            add(st);

            for (int i = pos; i < tkls->size(); ++i) {
                root_ptr<String> first = tkls->get(i)->as<String>();
                if (first->get_val() == "(") {
                    root_ptr<regular_node> l = st.top();
                    l->append(add(st).get_p());
                } else if (first->get_val() == ")") {
                    st.top()->mode = regular_node::AND;
                    if (i + 1 < tkls->size()) {
                        string nxt = tkls->get(i + 1)->as<String>()->get_val();
                        if (nxt == "*") {
                            st.top()->mode = regular_node::NONE_OR_MORE;
                        } else if (nxt == "?") {
                            st.top()->mode = regular_node::ONE_OR_NONE;
                        } else if (nxt == "+") {
                            st.top()->mode = regular_node::ONE_OR_MORE;
                        }
                        i += 1;
                    }
                    st.pop();
                    st.pop();
                } else if (first->get_val() == "|") {
                    root_ptr<regular_node> old = st.top();
                    st.pop();
                    st.top()->append(old.get_p());

                    root_ptr<regular_node> nn = alloc(regular_node);
                    nn->mode = regular_node::AND;
                    st.push(nn);
                } else {
                    if (first->get(0) == '\'') {
                        first = splice(first, 1, -1);
                    }
                    root_ptr<regular_node> nd = alloc(regular_node);
                    nd->mode = regular_node::NODE;
                    nd->name = first->get_val();
                    st.top()->append(nd.get_p());
                }

            }
            while (st.size() > 1)
                st.pop();
            return st.top();
        }

        void add_rule(const string &str) {
            input.push_back(str);
        }

        void build_all() {
            for (const string &s : input) {

                root_ptr<list> l = split(s);
                string name = l->get(0)->as<String>()->get_val();

                int start = 0;
                bool replaceable = false;
                int index;
                for(index = 0; index < l->size(); ++index)
                {
                    if(l->get(index)->as<String>()->get_val() == ":")
                        start = index + 1;
                    if(l->get(index)->as<String>()->get_val() == "REPLACEABLE")
                        replaceable = true;
                }
                root_ptr<regular_node> rnode = build(l, start);
                rnode->replaceable = replaceable;
                rule_map.insert(std::make_pair(name, rnode));

            }
        }


        void print_all() {
            for (auto e : rule_map) {
                std::cout << e.first << " := " << e.second->to_string() << std::endl;
            }
        }


        std::pair<root_ptr<list>, int> match_node(const root_ptr<regular_node> &root_node, const int token_pos) {
//            std::cerr << "entered " << root_node->to_string() << std::endl;
            int pos = token_pos;
            root_ptr<list> matched = alloc(list);

            int rep;
            for (rep = 0; rep < root_node->r_end(); ++rep) {
                root_ptr<list> matched_subrules = alloc(list);

                for (int index = 0; index < root_node->size(); ++index) {
                    root_ptr<regular_node> s = root_node->get(index);
                    if (s->name.size()) {
                        std::pair<root_ptr<node>, int> result = match_rule(s->name, pos);
                        if (!result.first)
                            goto exit;

                        matched_subrules->append(result.first.get_p());
                        pos = result.second;
                    } else {
                        std::pair<root_ptr<list>, int> result = match_node(s, pos);
                        if (!result.first && (root_node->mode != regular_node::OR || index == root_node->size() - 1))
                            goto exit;
                        if (result.first || root_node->mode == regular_node::OR) {
//                            std::cerr << result.first->to_string() << std::endl;
                            for (auto e : *result.first)
                                if(e->as<node>()->get_type() != EMPTY && e->as<node>()->get_type() != EPS)
                                    matched_subrules->append(e);
                            pos = result.second;
                        }
                    }
                }
                for (object * e : *matched_subrules)
                {
                    node *n = e->as<node>();
                    matched->append(e);
                }
            }
            exit:;
            if (rep >= root_node->r_begin()) {
                return std::make_pair(matched, pos);
            }
//            std::cerr << "failed " << root_node->to_string() << std::endl;
            return std::make_pair(matched, pos);

        }

        std::vector<GenLang::string> st;

        std::pair<root_ptr<node>, int> match_rule(const string &rule_name, const int token_pos) {
            autostack holder(st, rule_name);
//            for (auto &e : st) {
//                std::cerr << e << " ";
//            }
//            std::cerr << std::endl;
//            std::cerr << "token pos " << token_pos << std::endl;

            if (token_pos < tokens.size()) {
                if (tokens[token_pos]->match(rule_name)) {
                    return std::make_pair(alloc(node, tokens[token_pos]), token_pos + 1);
                }
            } else if (rule_name == EPS)
            {
                root_ptr<list> empty = alloc(list);
                root_ptr<node> nd = alloc(node, EPS, empty);
                return std::make_pair(nd, token_pos);
            }
            auto range = rule_map.equal_range(rule_name);
            for (auto it = range.first; it != range.second; ++it) {

                auto result = match_node(it->second, token_pos);
                root_ptr<list> l = result.first;
                if (!l)
                    continue;
                // todo here
//                if(it->second->replaceable && l->size() == 1)
//                    return std::make_pair(l->get(0), result.second);

                root_ptr<node> nd = alloc(node, it->first, l);
                return std::make_pair(nd, result.second);
            }
            if(rule_name == EMPTY)
            {
                root_ptr<list> empty = alloc(list);
                root_ptr<node> nd = alloc(node, EMPTY, empty);
                return std::make_pair(nd, token_pos);
            }
            holder.set(false);
            return std::make_pair((node *) NULL, token_pos);
        }

        void put_token(const root_ptr<token> &tk) {
            tokens.push_back(tk);
        }
    };

    //  example :  multi : add ( '*' add_rule ) *
    //  example :  add_rule   : num ( ['+' '-'] num ) *
    //  example :  add_rule   : num ( ('+' | '-') num ) *

}


#endif //GENLANG_REGULAR_PARSER_H
