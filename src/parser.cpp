#include "genlang/object.h"
#include "genlang/autorun.h"
#include "genlang/runtime_support.h"
#include "genlang/parser.h"
using namespace GenLang;
static void reg()
{
    add_type("node", "map_object", typeid(node));
}
static autorun run(reg);
struct item {
    string name;
    string rule;
    bool left;
};
namespace GenLang
{
    class Parser
    {
    public:
        // std::vector<item> items;
        // std::vector<token> tokens;
        // std::pair<node *, int> match(const char *rule_name, int tokens_pos)
        // {
        //     if (tokens_pos and rule_name == tokens[tokens_pos].name)
        //         return RuleMatch(tokens[0], tokens[1:])
        //     for expansion in rule_map.get(rule_name, ()):
        //         remaining_tokens = tokens
        //         matched_subrules = []
        //         for subrule in expansion.split():
        //             matched, remaining_tokens = match(subrule, remaining_tokens)
        //             if not matched:
        //                 break   # 运气不好，跳出循环，处理下一个扩展定义!
        //             matched_subrules.append(matched)
        //         else:
        //             return RuleMatch(rule_name, matched_subrules), remaining_tokens
        //     return None, None   # 无匹配结果
        // }
    };

}
