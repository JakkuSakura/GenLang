//
// Created by jack on 12/5/18.
//

#include "genlang/complier/regular_parser.h"

int GenLang::regular_node::r_begin() {
    switch (rep) {
        default:
        case ANY:
        case OPTIONAL:
            return rep_begin = 0;
        case ONCE_OR_MORE:
        case ONCE:
            return rep_begin = 1;
        case REP:
            return rep_begin;

    }
}

int GenLang::regular_node::r_end() {
    switch (rep) {
        default:
        case OPTIONAL:
        case ONCE:
            return rep_end = 1;
        case ONCE_OR_MORE:
        case ANY:
            return rep_end = INF;
        case REP:
            return rep_end;

    }
}

GenLang::string GenLang::regular_node::to_string() const {
    string str;
    if (mode == TOKEN) {
        str += "'";
        str += val;
        str += "'";
        str += " ";
    } else {
        if (size() >= 2)
            str += mode == OR ? "[ " : "( ";
        for (int i = 0; i < size(); ++i) {
            str += get(i)->to_string();
        }
        if (size() >= 2)
            str += mode == OR ? "] " : ") ";
    }

    switch (rep) {
        case ONCE_OR_MORE:
            str += "+ ";
            break;
        case OPTIONAL:
            str += "? ";
            break;
        case ANY:
            str += "* ";
            break;
        default:
            break;
    }
    if (errorat)
        str += "# ";
    if (extract)
        str += "& ";

    return str;
}

GenLang::root_ptr<GenLang::token> GenLang::regular_parser::get_token(int i) {
    return tokens[i];
}

GenLang::root_ptr<GenLang::regular_node>
GenLang::regular_parser::new_rp_node(std::stack<GenLang::root_ptr<GenLang::regular_node>> &st,
                                     regular_node::ContainerType type) {
    root_ptr<regular_node> r = alloc(regular_node);
    st.push(r);
    st.top()->mode = type;
    return r;
}

GenLang::root_ptr<GenLang::regular_node>
GenLang::regular_parser::build(const GenLang::root_ptr<GenLang::list> &tkls, int pos) {
    std::stack<root_ptr<regular_node> > st;
//            std::cerr << tkls->to_string() << std::endl;
    root_ptr<regular_node> root = new_rp_node(st, regular_node::AND);

    for (int i = pos; i < tkls->size(); ++i) {
        root_ptr<String> first = tkls->get(i)->as<String>();
        root_ptr<regular_node> top_ele = st.top();
//        std::cerr << first->to_string() << std::endl;
        if (first->get_val() == "(") {
            top_ele->append(new_rp_node(st, regular_node::AND).get_p());
        } else if (first->get_val() == "[") {
            top_ele->append(new_rp_node(st, regular_node::OR).get_p());
        } else if (first->get_val() == ")") {
            st.pop();
        } else if (first->get_val() == "]") {
            st.pop();
        } else if (first->get_val() == "*") {
            top_ele->get(-1)->rep = regular_node::ANY;
        } else if (first->get_val() == "?") {
            top_ele->get(-1)->rep = regular_node::OPTIONAL;
        } else if (first->get_val() == "+") {
            top_ele->get(-1)->rep = regular_node::ONCE_OR_MORE;
        } else if (first->get_val() == "#") {
            top_ele->get(-1)->errorat = true;
        } else if (first->get_val() == "&") {
            top_ele->get(-1)->extract = true;
        } else {
            if (first->get(0) == '\'') {
                root_ptr<String> str = first;
                first = alloc(String, str->get_val().sub_str(1, -1));
            }
            root_ptr<regular_node> nd = alloc(regular_node);
            nd->mode = regular_node::TOKEN;
            nd->val = first->get_val();
            top_ele->append(nd.get_p());
        }
//        std::cerr << root->to_string() << std::endl;
    }

    return root;

}

void GenLang::regular_parser::add_rule(const GenLang::string &str) {
    input.push_back(str);
    root_ptr<list> l = split(str);
    string name = l->get(0)->as<String>()->get_val(), tip;

    int start = 0;
    bool replaceable = false, errorat = false;
    int index;
    for (index = 0; index < l->size(); ++index) {
        string first = l->get(index)->as<String>()->get_val();
        if (first == ":") {
            start = index + 1;
            break;
        }
        if (first == "&")
            replaceable = true;
        else if (first[0] == '\'') {
            tip = first.sub_str(1, -1);
        }
    }
    root_ptr<regular_node> rnode = build(l, start);

    rnode->errorat = errorat;
    rnode->replaceable = replaceable;
    rnode->tip = tip;
    rule_map.insert(std::make_pair(name, rnode));

}

void GenLang::regular_parser::print_all() {
    for (auto e : rule_map) {
        std::cout << e.first << " := " << e.second->to_string() << std::endl;
    }
}

std::pair<GenLang::root_ptr<GenLang::list>, int>
GenLang::regular_parser::match_node(const GenLang::root_ptr<GenLang::regular_node> &root_node, const int token_pos) {
    autostack holder(st2, root_node->to_string(), "match_node");
//    show_auto_stack
    int pos = token_pos;
    root_ptr<list> matched = alloc(list);

    int rep;
    for (rep = 0; rep < root_node->r_end(); ++rep) {
        root_ptr<list> matched_subrules = alloc(list);

        for (int index = 0; index < root_node->size(); ++index) {
            root_ptr<regular_node> s = root_node->get(index);

            list *lst = NULL;
            node *nde = NULL;
            int newpos;
            if (s->mode == regular_node::TOKEN) {
                std::pair<root_ptr<node>, int> result = match_rule(s->val, pos);
                nde = result.first;
                newpos = result.second;
            } else {
                std::pair<root_ptr<list>, int> result = match_node(s, pos);
                lst = result.first;
                newpos = result.second;
            }
            if (!(lst || nde) && (root_node->mode != regular_node::OR || index == root_node->size() - 1))
                goto exit;

            if (lst) {
                for (auto e : *lst)
                    if (e->as<node>()->get_type() != EMPTY)
                        matched_subrules->append(e);
                pos = newpos;
            } else if (nde) {
                if (s->extract)
                    for (auto e : *nde->get_matched())
                        matched_subrules->append(e);
                else
                    matched_subrules->append(nde);
                pos = newpos;
            }

            if ((lst || nde) && root_node->mode == regular_node::OR)
                break;
        }
        for (object *e : *matched_subrules) {
            matched->append(e);
        }
    }
    exit:
    if (rep >= root_node->r_begin()) {
        return std::make_pair(matched, pos);
    } else {
        if (root_node->errorat) {
            char buf[1000];
            sprintf(buf, "parse error : missing %s at %d \n %s", root_node->to_string().get_val().c_str(), token_pos,
                    token_pos < tokens.size() ? tokens[token_pos]->to_string().get_val().c_str() : "");
            throw string(buf);
        }
    }
    holder.set(false);
    return std::make_pair((list *) nullptr, pos);
}

std::pair<GenLang::root_ptr<GenLang::node>, int>
GenLang::regular_parser::match_rule(const GenLang::string &rule_name, const int token_pos) {
    static root_ptr<list> empty = alloc(list);
    static root_ptr<node> empty_node = alloc(node, EMPTY, empty, "");
    autostack holder(st, rule_name, "match_rule");
    holder.print_stack();
//            std::cerr << "token pos " << token_pos << std::endl;

    if (token_pos < tokens.size()) {
        if (tokens[token_pos]->match(rule_name)) {
            return std::make_pair(alloc(node, tokens[token_pos]), token_pos + 1);
        }
    } else if (rule_name == EPS) {
        return std::make_pair(empty_node, token_pos);
    }
    auto range = rule_map.equal_range(rule_name);
    for (auto it = range.first; it != range.second; ++it) {

        auto result = match_node(it->second, token_pos);
        root_ptr<list> l = result.first;

        if (!l) {
            continue;
        }

        if (it->second->replaceable) {
            if (l->size() == 1)
                return std::make_pair(l->get(0)->as<node>(), result.second);
            else if (l->size() == 0)
                return std::make_pair(empty_node, result.second);
        }

        root_ptr<node> nd = alloc(node, it->first, l, it->second->tip);
        return std::make_pair(nd, result.second);
    }
    if (rule_name == EMPTY) {
        return std::make_pair(empty_node, token_pos);
    }
    holder.set(false);
    return std::make_pair((node *) nullptr, token_pos);
}

void GenLang::regular_parser::put_token(const GenLang::root_ptr<GenLang::token> &tk) {
    tokens.push_back(tk);
}
