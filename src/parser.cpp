#include <genlang/complier/token.h>
#include "genlang/autorun.h"
#include "genlang/parser.h"

using namespace GenLang;

static void reg() {
    add_type("node", "map_object", typeid(node));
}

static autorun run(reg);
struct item {
    string name;
    string rule;
    bool left;
};
namespace GenLang {
    // todo
    class Parser {
    public:
        std::multimap<string, item> rule_map;
        std::vector<token> tokens;

        std::pair<node *, int> match(const string &rule_name, int token_pos) {
            if(token_pos < tokens.size())
            {
                if (rule_name == ((String *) tokens[token_pos].get("name"))->get_val()) {
                    node *nd = alloc(node, tokens[token_pos]);
                    return std::make_pair(nd, token_pos + 1);
                }
                auto range = rule_map.equal_range(rule_name);
                for (auto it = range.first; it != range.second; ++it) {
                    auto expansion = *it;
                    std::vector<node *> matched_subrules;
                    bool ok = true;
                    int newtokenpos = token_pos;
                    for (auto subrule : expansion.split()) {
                        auto pr = match(subrule, token_pos + 1);
                        auto matched = pr.first;
                        newtokenpos = pr.second;
                        if (!matched) {
                            ok = false;
                            break;
                        }
                        matched_subrules.push_back(matched)

                    }
                    if (ok)
                        return std::make_pair(alloc(node, rule_name, matched_subrules), newtokenpos);
                }
            }
            return std::make_pair((node *) NULL, 0);
        }
    };

}
