#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
#include "genlang/complier/token.h"
#include "genlang/complier/lexer.h"

namespace GenLang {
    struct node : public map_object {
        std::vector<node *> matched;

        node(string type) {
            put(string("type"), alloc(String, type));
        }

        node(token *tk) {
            put(string("type"), tk->get_token_type());
            put(string("val"), tk->get_val());
        }

        node(string type, std::vector<node *> &matched_l) {
            matched = matched_l;
            put(string("type"), alloc(String, type));
        }
    };

    struct item {
        string name;
        string rule;
        bool left;
    };

    class parser {
        FILE *fout;
        scanner scan;
    public:
        parser(FILE *fin, FILE *fout);
        std::multimap<string, item> rule_map;
        std::vector<token *> tokens;

        std::pair<node *, int> match(const string &rule_name, int token_pos);

        void add_rule(const string &name, const string &rule, bool left);
        void scan_src()
        {
            token *tk;
            while ((tk = scan.get_token()) && (tk->get_token_type() != TokenType::TEOF))
            {
                tokens.push_back(tk);
            }
        }
    };

} // GenLang