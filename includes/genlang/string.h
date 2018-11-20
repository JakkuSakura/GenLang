#include <utility>

#ifndef GENLANG_STRING_H
#define GENLANG_STRING_H

#include <string>
#include <cstring>
#include <memory>
#include <iostream>

namespace GenLang {
    class string_builder {
        std::string s;
    public:

        string_builder(const char *cs) : s(cs) {}

        string_builder(char ch) { s += ch; }

        string_builder(std::string cs) : s(std::move(cs)) {}

        string_builder() : s() {}

        string_builder(const string_builder &s2) : s(s2.s) {}

        friend string_builder operator+(const string_builder &a, const string_builder &b) {
            return string_builder(a.s + b.s);
        }

        friend bool operator<(const string_builder &a, const string_builder &b) {
            return a.s < b.s;
        }

        friend bool operator==(const string_builder &a, const string_builder &b) {
            return a.s == b.s;
        }

        friend bool operator!=(const string_builder &a, const string_builder &b) {
            return a.s != b.s;
        }

        string_builder &operator+=(const string_builder &b) {
            s += b.s;
            return *this;
        }

        string_builder &operator+=(char ch) {
            s += ch;
            return *this;
        }

        string_builder &operator=(const string_builder &b) {
            s = b.s;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const string_builder &s) {
            out << s.get_val();
            return out;
        }

        std::string &get_val() {
            return s;
        }

        const std::string &get_val() const {
            return s;
        }
        friend void swap(string_builder &a, string_builder &b) {
            std::swap(a.s, b.s);
        }
    };

    class string {
        std::shared_ptr<const string_builder> ptr;
    public:
        string(const char *cs) : ptr(std::make_shared<const string_builder>(cs)) {}

        string(const std::string &cs) : ptr(std::make_shared<const string_builder>(cs)) {}

        string() : ptr(std::make_shared<const string_builder>("")) {}

        string(const string_builder &s2) : ptr(std::make_shared<const string_builder>(s2.get_val())) {}

        string(const string &s2) : ptr(s2.ptr) {}

        string &operator=(const string &s2) {
            ptr = s2.ptr;
            return *this;
        }

        const string_builder &get_val() const {
            return *this;
        }

        operator const string_builder &() const {
            return *ptr;
        }

        operator const std::string &() const {
            return get_val().get_val();
        }

        std::string::const_iterator begin() const {
            return get_val().get_val().begin();
        }

        std::string::const_iterator end() const {
            return get_val().get_val().end();
        }

        friend string operator+(const string &a, const string &b) {
            return string(a.get_val() + b.get_val());
        }

        friend bool operator<(const string &a, const string &b) {
            return a.get_val() < b.get_val();
        }

        friend bool operator==(const string &a, const string &b) {
            return a.get_val() == b.get_val();
        }

        friend bool operator!=(const string &a, const string &b) {
            return a.get_val() != b.get_val();
        }

        friend std::ostream &operator<<(std::ostream &out, const string &s) {
            out << s.get_val();
            return out;
        }

    };

}
#endif