#include "genlang/utils.h"

namespace GenLang {

    list *split(const string &str) {
        list *l = alloc(list);
        string sb;
        for (char c : str) {
            if (!isspace(c)) sb += c;
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

    bool is_upper(const string &str) {
        for (char e : str) {
            if(!isupper(e))
                return false;
        }
        return true;
    }
    bool is_alpha(const string &str) {
        for (char e : str) {
            if(!isalpha(e))
                return false;
        }
        return true;
    }
}
