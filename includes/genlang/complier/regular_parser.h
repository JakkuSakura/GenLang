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
            NONE, AND, OR, ONE_OR_NONE, ONE_OR_MORE, NONE_OR_MORE, NODE
        } mode;
        string name;
        int pos = 0;
        list *matched = alloc(list);

        string to_string() const override {
            string str;
            if (name.size()) {
                if(name == "*" || name == "?" || name == "*")
                    str += "'";
                str += name;
                if(name == "*" || name == "?" || name == "*")
                    str += "'";
                str += " ";
            } else {
                bool first = true;
                if(size() >= 2)
                    str += "( ";

                for (auto e : *this) {
                    str += e->to_string();
                    if(!first && mode == OR)
                        str += "| ";

                    first = false;
                }
                if(size() >= 2)
                    str += ") ";
            }

            switch (mode)
            {
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

    class regular_parser {
        std::vector<string> input;
        std::multimap<string, root_ptr<regular_node> > rule_map;
        std::vector<root_ptr<token> > tokens;

    public:
        root_ptr<regular_node> build(const root_ptr<list> &tkls, int pos) {
            std::stack<root_ptr<regular_node> > st;

            st.push(alloc(regular_node));
            st.top()->mode = regular_node::AND;

            for (int i = pos; i < tkls->size(); ++i) {
                root_ptr<String> first = tkls->get(i)->as<String>();
                if (first->get(0) == '\'') {
                    first = splice(first, 1, -1);
                    root_ptr<regular_node> nd = alloc(regular_node);
                    nd->mode = regular_node::NODE;
                    nd->name = first->get_val();
                    st.top()->append(nd.get_p());
                } else if (first->get_val() == "(") {
                    root_ptr<regular_node> lst = alloc(regular_node);
                    st.top()->as<regular_node>()->append(lst.get_p());
                    st.push(lst);
                } else if (first->get_val() == ")") {
                    st.top()->mode = regular_node::AND;
                    if(i + 1 < tkls->size())
                    {
                        string nxt = tkls->get(i + 1)->as<String>()->get_val();
                        if ( nxt == "*") {
                            st.top()->mode = regular_node::NONE_OR_MORE;
                        } else if (nxt == "?") {
                            st.top()->mode = regular_node::ONE_OR_NONE;
                        } else if (nxt == "+") {
                            st.top()->mode = regular_node::ONE_OR_MORE;
                        }
                        i += 1;
                    }
                    st.pop();
                } else {
                    root_ptr<regular_node> nd = alloc(regular_node);
                    nd->mode = regular_node::NODE;
                    nd->name = first->get_val();
                    st.top()->append(nd.get_p());
                }

            }
            return st.top();
        }

        void add_rule(const string &str) {
            input.push_back(str);
        }

        void build_all() {
            for (const string &s : input) {

                root_ptr<list> l = split(s);
                string name = l->get(0)->as<String>()->get_val();

                root_ptr<regular_node> rnode = build(l, 2);

                rule_map.insert(std::make_pair(name, rnode));

            }
        }


        void print_all() {
            for(auto e : rule_map)
            {
                std::cout << e.first << " := " << e.second->to_string() << std::endl;
            }
        }
        std::vector<GenLang::string> st;
        std::pair<root_ptr<node>, int> match_rule(const string &rule_name, int token_pos) {
            // todo : finish this : stack and pos and or
            autostack holder(st, rule_name);
            for (auto &e : st) {
                std::cerr << e << " ";
            }
            std::cerr << std::endl;
            std::cerr << "token pos " << token_pos << std::endl;

            if (token_pos < tokens.size()) {
                if (tokens[token_pos]->match(rule_name)) {
                    return std::make_pair(alloc(node, tokens[token_pos]), token_pos + 1);
                }

                auto range = rule_map.equal_range(rule_name);
                for (auto it = range.first; it != range.second; ++it) {



                    std::vector<root_ptr<regular_node> > stk;
                    stk.push_back(it->second);

                    for(int index = 0; index < stk.back()->size(); ++index)
                    {
                        root_ptr<regular_node> s = stk.back()->get(index);
                        if (s->name.size())
                        {
                            auto pr = match_rule(s->name, newtokenpos);
                            root_ptr<node> matched = pr.first;
                            newtokenpos = pr.second;
                            if(stk.back()->mode == regular_node::AND && !matched) {
                                index = 0;
                                stk.pop_back();
                                continue;
                            }
                            if(stk.back()->mode == regular_node::NONE_OR_MORE && !matched) {
                                continue;
                            }
                            if(stk.back()->mode == regular_node::ONE_OR_MORE && !matched) {
                                if(matched_subrules->size() == 0)
                                {
                                    stk.pop_back();
                                }
                                continue;
                            }
                            if(stk.back()->mode == regular_node::ONE_OR_NONE && !matched) {
                                stk.pop_back();
                                continue;
                            }

                            matched_subrules->append(matched.get_p());

                            if(stk.back()->mode == regular_node::ONE_OR_NONE) {
                                if (matched_subrules->size() > 1)
                                {
                                    stk.pop_back();
                                    continue;
                                }
                            }

                            if(stk.back()->mode == regular_node::OR && matched) {
                                stk.pop_back();
                                continue;
                            }
                        } else
                        {
                            if(stk.back()->mode == regular_node::OR && matched_subrules->size())
                            {
                                stk.pop_back();
                            }
                            stk.push_back(s);
                        }
                    }


                    if (!stk.empty()) {
                        root_ptr<list> matched_subrules = alloc(list);
                        // todo ...sth
                        std::pair<root_ptr<node>, int> t = std::make_pair(alloc(node, rule_name, matched_subrules),
                                                                          newtokenpos);
                        return t;
                    }
                }
            }
            holder.set(false);
            return std::make_pair((node *) NULL, token_pos);
        }
        void put_token(root_ptr<token> tk)
        {
            tokens.push_back(tk);
        }
    };

    //  example :  multi : add ( '*' add_rule ) *
    //  example :  add_rule   : num ( ['+' '-'] num ) *
    //  example :  add_rule   : num ( ('+' | '-') num ) *

}


#endif //GENLANG_REGULAR_PARSER_H
