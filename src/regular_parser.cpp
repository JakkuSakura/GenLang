//
// Created by jack on 12/5/18.
//

#include "genlang/complier/regular_parser.h"

int GenLang::regular_node::r_begin() {
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

int GenLang::regular_node::r_end() {
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

GenLang::string GenLang::regular_node::to_string() const {
    string str;
    if (name.size()) {

        str += "'";
        str += name;
        str += "'";
        str += " ";
    } else {
        if (size() >= 2 || mode == NONE_OR_MORE || mode == ONE_OR_MORE || mode == ONE_OR_NONE || mode == REP)
            str += "( ";
        if (size()) str += get(0)->to_string();
        for (int i = 1; i < size(); ++i) {
            if (mode == OR)
                str += "| ";
            str += get(i)->to_string();
        }
        if (size() >= 2 || mode == NONE_OR_MORE || mode == ONE_OR_MORE || mode == ONE_OR_NONE || mode == REP)
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

GenLang::root_ptr<GenLang::token> GenLang::regular_parser::get_token(int i) {
    return tokens[i];
}

GenLang::root_ptr<GenLang::regular_node>
GenLang::regular_parser::add(std::stack<GenLang::root_ptr<GenLang::regular_node>> &st) {
    root_ptr<regular_node> root = alloc(regular_node);
    st.push(root);
    st.top()->mode = regular_node::OR;
    root_ptr<regular_node> r = alloc(regular_node);
    st.top()->append(r.get_p());
    st.push(r);
    st.top()->mode = regular_node::AND;
    return root;
}

GenLang::root_ptr<GenLang::regular_node>
GenLang::regular_parser::build(const GenLang::root_ptr<GenLang::list> &tkls, int pos) {
    std::stack<root_ptr<regular_node> > st;
//            std::cerr << tkls->to_string() << std::endl;
    root_ptr<regular_node> root = add(st);

    for (int i = pos; i < tkls->size(); ++i) {
//                std::cerr << root->to_string() << std::endl;
        root_ptr<String> first = tkls->get(i)->as<String>();
//                std::cerr << first->to_string() << std::endl;
        if (first->get_val() == "(") {
            root_ptr<regular_node> l = st.top();
            l->append(add(st).get_p());
        } else if (first->get_val() == ")") {
            if (i + 1 < tkls->size()) {
                string nxt = tkls->get(i + 1)->as<String>()->get_val();
                i += 1;
                if (nxt == "*") {
                    st.top()->mode = regular_node::NONE_OR_MORE;
                } else if (nxt == "?") {
                    st.top()->mode = regular_node::ONE_OR_NONE;
                } else if (nxt == "+") {
                    st.top()->mode = regular_node::ONE_OR_MORE;
                } else {
                    i -= 1;
                }
            }
            st.pop();
            st.pop();
        } else if (first->get_val() == "|") {
            root_ptr<regular_node> old = st.top();
            st.pop();

            root_ptr<regular_node> nn = alloc(regular_node);
            nn->mode = regular_node::AND;
            st.top()->append(nn.get_p());
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
//            std::cerr << root->to_string() << std::endl;
    while (st.size() > 1)
        st.pop();
    return st.top();

}

void GenLang::regular_parser::add_rule(const GenLang::string &str) {
    input.push_back(str);
}

void GenLang::regular_parser::build_all() {
    for (const string &s : input) {

        root_ptr<list> l = split(s);
        string name = l->get(0)->as<String>()->get_val();

        int start = 0;
        bool replaceable = false, errorat = false;
        int index;
        for (index = 0; index < l->size(); ++index) {
            if (l->get(index)->as<String>()->get_val() == ":")
                start = index + 1;
            if (l->get(index)->as<String>()->get_val() == "REPLACEABLE")
                replaceable = true;
            if (l->get(index)->as<String>()->get_val() == "ERRORAT")
                errorat = true;

        }
        root_ptr<regular_node> rnode = build(l, start);
        rnode->replaceable = replaceable;
        rnode->errorat = errorat;
        rule_map.insert(std::make_pair(name, rnode));

    }
}

void GenLang::regular_parser::print_all() {
    for (auto e : rule_map) {
        std::cout << e.first << " := " << e.second->to_string() << std::endl;
    }
}

std::pair<GenLang::root_ptr<GenLang::list>, int>
GenLang::regular_parser::match_node(const GenLang::root_ptr<GenLang::regular_node> &root_node, const int token_pos,
                                    bool fatal) {
    autostack holder(st2, root_node->to_string());
    int pos = token_pos;
    root_ptr<list> matched = alloc(list);

    int rep;
    for (rep = 0; rep < root_node->r_end(); ++rep) {
        root_ptr<list> matched_subrules = alloc(list);

        for (int index = 0; index < root_node->size(); ++index) {
            root_ptr<regular_node> s = root_node->get(index);
            if (s->name.size()) {
                std::pair<root_ptr<node>, int> result = match_rule(s->name, pos, fatal && (root_node->mode == regular_node::AND));
                if (!result.first)
                    goto exit;

                matched_subrules->append(result.first.get_p());
                pos = result.second;
            } else {
                std::pair<root_ptr<list>, int> result = match_node(s, pos, fatal);
                if (!result.first && (root_node->mode != regular_node::OR || index == root_node->size() - 1))
                    goto exit;

                if (result.first) {
//                            std::cerr << result.first->to_string() << std::endl;
                    for (auto e : *result.first)
                        if (e->as<node>()->get_type() != EMPTY && e->as<node>()->get_type() != EPS)
                            matched_subrules->append(e);
                    pos = result.second;
                }

                if (result.first && root_node->mode == regular_node::OR)
                    break;
            }
        }
        for (object *e : *matched_subrules) {
            node *n = e->as<node>();
            matched->append(e);
        }
    }
    exit:;
    if (rep >= root_node->r_begin()) {
        return std::make_pair(matched, pos);
    }
    holder.set(false);
    return std::make_pair((list *) nullptr, pos);
}

std::pair<GenLang::root_ptr<GenLang::node>, int>
GenLang::regular_parser::match_rule(const GenLang::string &rule_name, const int token_pos, bool fatal) {
    autostack holder(st, rule_name);
//            holder.print_stack();
//            std::cerr << "token pos " << token_pos << std::endl;

    if (token_pos < tokens.size()) {
        if (tokens[token_pos]->match(rule_name)) {
            return std::make_pair(alloc(node, tokens[token_pos]), token_pos + 1);
        }
    } else if (rule_name == EPS) {
        root_ptr<list> empty = alloc(list);
        root_ptr<node> nd = alloc(node, EPS, empty);
        return std::make_pair(nd, token_pos);
    }
    auto range = rule_map.equal_range(rule_name);
    for (auto it = range.first; it != range.second; ++it) {

        auto result = match_node(it->second, token_pos, fatal);
        root_ptr<list> l = result.first;

        if (!l) {
            if (fatal && it->second->errorat) {
                char buf[1000];
                sprintf(buf, "parse error at %d \n %s", token_pos,
                        token_pos < tokens.size() ? tokens[token_pos]->to_string().get_val().c_str() : "");
                throw string(buf);
            }
            continue;
        }

        if (it->second->replaceable && l->size() == 1)
            return std::make_pair(l->get(0), result.second);

        root_ptr<node> nd = alloc(node, it->first, l);
        return std::make_pair(nd, result.second);
    }
    if (rule_name == EMPTY) {
        root_ptr<list> empty = alloc(list);
        root_ptr<node> nd = alloc(node, EMPTY, empty);
        return std::make_pair(nd, token_pos);
    }
    holder.set(false);
    return std::make_pair((node *) nullptr, token_pos);
}

void GenLang::regular_parser::put_token(const GenLang::root_ptr<GenLang::token> &tk) {
    tokens.push_back(tk);
}
