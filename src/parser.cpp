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

    std::pair<node *, int> parser::match(const string &rule_name, int token_pos) {
        if (token_pos < tokens.size()) {
            if (tokens[token_pos]->match(rule_name)) {
                node *nd = alloc(node, tokens[token_pos]);
                return std::make_pair(nd, token_pos + 1);
            }
            auto range = rule_map.equal_range(rule_name);
            for (auto it = range.first; it != range.second; ++it) {
                string expansion = it->second.rule;
                std::vector<node *> matched_subrules;
                bool ok = true;
                int newtokenpos = token_pos;
                for (auto subrule : *split(expansion)) {
                    auto pr = match(*(string *) subrule, token_pos + 1);
                    auto matched = pr.first;
                    newtokenpos = pr.second;
                    if (!matched) {
                        ok = false;
                        break;
                    }
                    matched_subrules.push_back(matched);
                }
                if (ok)
                    return std::make_pair(alloc(node, rule_name, matched_subrules), newtokenpos);
            }
        }
        return std::make_pair((node *) NULL, 0);
    }

    void parser::add_rule(const string &name, const string &rule, bool left) {
        item item1 = (item){name, rule, left};
        rule_map.insert(std::make_pair(name, item1));
    }

    parser::parser(FILE *fin, FILE *fout) : fout(fout), scan(fin)
    {
        std::cout << "inited parser" << std::endl;
    }
}
