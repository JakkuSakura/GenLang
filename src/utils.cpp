#include "genlang/utils.h"

namespace GenLang {

    list *split(const string &str) {
        list *l = alloc(list);
        string_builder sb;
        for (char c : str) {
            if (isalpha(c)) sb += c;
            else if (!sb.get_val().empty()) {
                l->append(alloc(String, sb));
                sb.get_val().clear();
            }
        }
        if (!sb.get_val().empty()) {
            l->append(alloc(String, sb));
            sb.get_val().clear();
        }
        return l;
    }

    list *split(const String &str) {
        return split(str.get_val());
    }
}
