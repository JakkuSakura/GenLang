#ifndef GENLANG_WRAPPER_H
#define GENLANG_WRAPPER_H

#include "genlang/object.h"
#include "genlang/string.h"

namespace GenLang {
    template<class T>
    class wrapper : public object {
        typedef wrapper<T> thisType;
        T val;

    protected:
        wrapper(T v) : val(v) {}

        wrapper() : val() {}

        wrapper(thisType *w) : val(w->val) {}

        operator const T() const {
            return get_val();
        }

        operator T() {
            return get_val();
        }

    public:
        const T &get_val() const {
            return val;
        }

        T &get_val() {
            return val;
        }
    };

    class Int : public wrapper<int> {
    public:
        Int(int v) : wrapper(v) {

        }

        Int() : wrapper(0) {

        }


        string to_string() const override {
            char buf[20];
            sprintf(buf, "%d", get_val());
            return buf;
        }
    };

    class Long : public wrapper<long long> {
    public:
        Long(long long v) : wrapper(v) {

        }

        Long() : wrapper(0LL) {

        }

        string to_string() const override {
            char buf[20];
            sprintf(buf, "%lld", get_val());
            return buf;
        }
    };

    class Char : public wrapper<char> {
    public:
        Char(char ch) : wrapper(ch) {

        }

        Char() = default;
    };

    class Double : public wrapper<double> {
    public:
        Double(double d) : wrapper(d) {
        }

        Double() = default;

        string to_string() const override {
            char buf[20];
            sprintf(buf, "%g", get_val());
            return buf;
        }
    };
    class StringBuilder : public wrapper<string_builder> {
    public:

        StringBuilder(const char *s) : wrapper(s) {
        }

        StringBuilder(StringBuilder *s) : wrapper(s->get_val()) {
        }

        StringBuilder(const string_builder &sb) : wrapper(sb) {}

        StringBuilder(const string &s) : wrapper(s) {}
        string to_string() const override {
            return "\"" + get_val() + "\"";
        }

        StringBuilder() = default;
    };

    class String : public wrapper<string> {
    public:
        String(const char *s) : wrapper(s) {
        }

        String(String *s) : wrapper(s) {
        }

        String(StringBuilder *s) : wrapper(s->get_val()) {
        }

        String(const string_builder &sb) : wrapper(sb) {}

        String(const string &s) : wrapper(s) {}

        String() = default;
        string to_string() const override {
            return "\"" + get_val() + "\"";
        }

    };


}
#endif