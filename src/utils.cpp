#include "genlang/utils.h"
#include  <algorithm>
#include <genlang/utils.h>


namespace GenLang {
    bool show_auto_stack = false;
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

    autostack::autostack(std::vector<GenLang::string> &st, GenLang::string s, GenLang::string name) : st(st) {
        st.push_back(s);
        ok = true;
        this->name = name;
        if(show_auto_stack)
            std::cerr << this->name <<  " ENTERED " << s << std::endl;
    }

    void autostack::set(bool s) {
        ok = s;
    }

    void autostack::print_stack() {
        if(show_auto_stack)
        {
            for (auto &e : st) {
                std::cerr << e << " ";
            }
            std::cerr << std::endl;

        }
    }

    autostack::~autostack() {
        if (show_auto_stack)
            std::cerr << this->name << " " << st.back() << " " << (ok ? "SUCCEEDED" : "FAILED") << std::endl;
        st.pop_back();
    }
}
