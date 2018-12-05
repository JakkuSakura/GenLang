#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include "genlang/container.h"
#include "genlang/string.h"
#include "genlang/runtime_support.h"
#include "genlang/wrapper.h"
#include "genlang/utils.h"
namespace GenLang {
    list *split(const string &str);
    list *split(const String &str);
    const int INF = 0x3f3f3f3f;
    root_ptr<list> splice(const root_ptr<list> &l, int left, int right);
    root_ptr<String> splice(const root_ptr<String> str, int left, int right);
    bool is_upper(const string &str);
    bool is_alpha(const string &str);
    class autostack {
        bool ok;
        std::vector<GenLang::string> &st;
    public:
        autostack(std::vector<GenLang::string> &st, GenLang::string s) : st(st) {
            st.push_back(s);
            ok = true;
        }

        void set(bool s) {
            ok = s;
        }

        ~autostack() {
            std::cerr << st.back() << " " << (ok ? "SUCCEEDED" : "FAILED") << std::endl;
            st.pop_back();
        }

    };
}

#endif //PROJECT_UTILS_H
