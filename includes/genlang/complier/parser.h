#include "genlang/object.h"
#include "genlang/wrapper.h"
#include "genlang/container.h"
#include "genlang/runtime_support.h"
#include "genlang/complier/token.h"
#include "genlang/complier/lexer.h"

namespace GenLang {
    struct node : public map_object {

        node(string type) {
            put("type", alloc(String, type));
        }

        node(token *tk) {
            put("type", tk->get_token_type());
            put("val", tk->get_val());
        }

        node(string type, list *matched_l) {
            put("type", alloc(String, type));
            put("matched", matched_l);
        }
    };

    struct item {
        string name;
        list *rule;
        int left;
        bool replacable;
    };

    class parser {
        FILE *fout;
        scanner scan;
    public:
        parser(FILE *fin, FILE *fout);
        std::multimap<string, item> rule_map;
        std::vector<token *> tokens;

        std::pair<node *, int> match(const string &rule_name, int token_pos);

        void add_rule(const string &name, const string &rule, int left, bool replacable);
        void scan_src();
    };

} // GenLang