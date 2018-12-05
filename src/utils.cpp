#include "genlang/utils.h"
#include  <algorithm>
#include <genlang/utils.h>


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



    root_ptr<list> splice(const root_ptr<list> &l, int left, int right) {
        root_ptr<list> nl = alloc(list);
        if(left < 0) left = l->size() + left;
        if(right < 0) right = l->size() + right;
        left = std::max(left, 0);
        right = std::min(right, l->size());
        for (int i = left; i < right; ++i) {
            nl->append(l->get(i));
        }
        return nl;
    }

    root_ptr<String> splice(const root_ptr<String> str, int left, int right) {
        root_ptr<String> nstr = alloc(String);
        if(left < 0) left = str->size() + left;
        if(right < 0) right = str->size() + right;
        left = std::max(left, 0);
        right = std::min(right, str->size());
        for (int i = left; i < right; ++i) {
            nstr->append(str->get(i));
        }
        return nstr;
    }

}
