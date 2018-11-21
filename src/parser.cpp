#include <cassert>
#include "genlang/complier/token.h"
#include "genlang/utils.h"
#include "genlang/autorun.h"
#include "genlang/complier/parser.h"

using namespace GenLang;

static void reg() {
    add_type("node", "map_object", typeid(node));
}


static autorun run(reg);
namespace GenLang {

    std::pair<node *, int> parser::match_rule(const string &rule_name, int token_pos) {
        using namespace rule_types;
        std::cerr << "trying " << rule_name << std::endl;
        if (token_pos < tokens.size()) {
            if (tokens[token_pos]->match(rule_name)) {
                node *nd = alloc(node, tokens[token_pos]);
                return std::make_pair(nd, token_pos + 1);
            }
            auto range = rule_map.equal_range(rule_name);
            for (auto it = range.first; it != range.second; ++it) {
                list *matched_subrules = alloc(list);
                bool ok = true;
                int newtokenpos = token_pos;
                node *matched;
                list *lst = it->second.rule;
                int type = it->second.type;
                switch (type) {
                    case NONE_OR_MORE:
                    case ONE_OR_MORE: { // rule+
                        std::pair<node *, int> pr;
                        int cnt = 0;
                        do {
                            pr = match_rule(((String *) lst->get(0))->get_val(), newtokenpos);
                            matched = pr.first;
                            newtokenpos = pr.second;
                            if (matched) {
                                matched_subrules->append(matched);
                                cnt += 1;
                            }
                        } while (matched);
                        if(type == 3 && cnt == 0)
                            ok = false;
                        break;
                    }
                    case LEFT: { // rule (mid rule)*
                        auto pr = match_rule(((String *) lst->get(0))->get_val(), newtokenpos);
                        matched = pr.first;
                        newtokenpos = pr.second;
                        if (matched) {
                            matched_subrules->append(matched);
                            do {
                                bool ok2 = false;
                                for (int i = 1; i < lst->size(); ++i) {
                                    pr = match_rule(((String *) lst->get(i))->get_val(), newtokenpos);
                                    matched = pr.first;
                                    newtokenpos = pr.second;
                                    if (matched) {
                                        ok2 = true;
                                        matched_subrules->append(matched);
                                        break;
                                    }
                                }
                                if (!ok2) {
                                    break;
                                }

                                pr = match_rule(((String *) lst->get(0))->get_val(), newtokenpos);
                                matched = pr.first;
                                newtokenpos = pr.second;
                                if (matched) {
                                    matched_subrules->append(matched);
                                } else {
                                    ok = false;
                                    break;
                                }
                            } while (matched);
                        } else {
                            ok = false;
                        }
                        break;
                    }
                    case NORMAL: { // any other rule
                        for (auto subrule : *lst) {
                            auto pr = match_rule(((String *) subrule)->get_val(), newtokenpos);
                            matched = pr.first;
                            newtokenpos = pr.second;
                            if (!matched) {
                                ok = false;
                                break;
                            }
                            matched_subrules->append(matched);
                        }
                        break;
                    }
                    default:
                        break;
                }
                if (ok) {
                    if (matched_subrules->size() == 1 && it->second.replacable)
                        return std::make_pair((node *) matched_subrules->get(0), newtokenpos);
                    else
                        return std::make_pair(alloc(node, rule_name, matched_subrules), newtokenpos);
                }
            }
        }
        return std::make_pair((node *) NULL, token_pos);
    }

    void parser::add_rule(const string &name, const string &rule, int type, bool replacable) {
        list *lst = split(rule);
        item item1 = (item) {name, lst, type, replacable};
        rule_map.insert(std::make_pair(name, item1));
    }

    parser::parser(FILE *fin, FILE *fout) : fout(fout), scan(fin) {
        std::cout << "inited parser" << std::endl;
    }

    void parser::scan_src() {
        token *tk;
        while ((tk = scan.get_token())) {
            tokens.push_back(tk);
        }
    }
}
