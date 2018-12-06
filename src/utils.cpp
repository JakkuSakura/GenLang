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

    string strip(const string &s) {
        string s2;
        int i = 0;
        for (; i < s.size(); ++i) {
            if(!isspace(s.get_val()[i]))
                break;
        }
        s2 += s.get_val().substr((unsigned long)i, INF);
        while (isspace(s2.get_val().back()))
            s2.get_val().pop_back();
        return s2;
    }

    autostack::autostack(std::vector<GenLang::string> &st, GenLang::string s) : st(st) {
        st.push_back(s);
        ok = true;
        if(show_auto_stack)
            std::cerr << "ENTERED " << s << std::endl;
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
            std::cerr << st.back() << " " << (ok ? "SUCCEEDED" : "FAILED") << std::endl;
        st.pop_back();
    }
}
