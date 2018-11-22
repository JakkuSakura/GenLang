#include <utility>

#ifndef GENLANG_STRING_H
#define GENLANG_STRING_H

#include <string>
#include <cstring>
#include <memory>
#include <iostream>

namespace GenLang {
    class string {
        std::string s;
    public:

        string(const char *cs) : s(cs) {}

        string(char ch) { s += ch; }

        string(std::string cs) : s(std::move(cs)) {}

        string() : s() {}

        string(const string &s2) : s(s2.s) {}

        friend string operator+(const string &a, const string &b) {
            return string(a.s + b.s);
        }

        friend bool operator<(const string &a, const string &b) {
            return a.s < b.s;
        }

        friend bool operator==(const string &a, const string &b) {
            return a.s == b.s;
        }

        friend bool operator!=(const string &a, const string &b) {
            return a.s != b.s;
        }

        string &operator+=(const string &b) {
            s += b.s;
            return *this;
        }

        string &operator+=(char ch) {
            s += ch;
            return *this;
        }

        string &operator=(const string &b) {
            s = b.s;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const string &s) {
            out << s.get_val();
            return out;
        }

        std::string &get_val() {
            return s;
        }

        const std::string &get_val() const {
            return s;
        }

        std::string::const_iterator begin() const {
            return s.begin();
        }

        std::string::const_iterator begin() {
            return s.begin();
        }

        std::string::const_iterator end() const {
            return s.end();
        }

        std::string::const_iterator end() {
            return s.end();
        }

        friend void swap(string &a, string &b) {
            std::swap(a.s, b.s);
        }
    };


}
#endif