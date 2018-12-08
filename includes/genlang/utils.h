#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include "genlang/container.h"
#include "genlang/string.h"
#include "genlang/runtime_support.h"
#include "genlang/wrapper.h"

namespace GenLang {

    list *split(const string &str);

    list *split(const String &str);

    const int INF = 0x3f3f3f3f;

    root_ptr<list> splice(const root_ptr<list> &l, int left, int right);

    bool is_upper(const string &str);

    bool is_alpha(const string &str);

    extern bool show_auto_stack;

    class autostack {
        bool ok;
        std::vector<GenLang::string> &st;
        string name;
    public:
        autostack(std::vector<GenLang::string> &st, GenLang::string s, GenLang::string name);

        void set(bool s);

        void print_stack();

        ~autostack();

    };
}

#endif //PROJECT_UTILS_H
