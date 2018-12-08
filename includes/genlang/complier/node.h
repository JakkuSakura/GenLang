//
// Created by jack on 12/5/18.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include "genlang/complier/token.h"
namespace GenLang{
    struct node : public map_object {

        node(token *tk) {
            set_unordered(true);
            put("type", tk->get_token_type());
            put("val", tk->get_val());
        }

        node(const string &type, list *matched_l, const string &tip) {
            set_unordered(true);
            put("type", alloc(String, type));
            put("matched", matched_l);
            put("tip", alloc(String, tip));
        }
        const string &get_type()
        {
            return get("type")->as<String>()->get_val();
        }
        list *get_matched()
        {
            return get("matched")->as<list>();
        }
        const string &get_str_val()
        {
            return get("val")->as<String>()->get_val();
        }
        bool has(const string &s)
        {
            if(get("matched"))
            {
                if(get_type() == s)
                    return true;
                for(auto e : *this->get_matched())
                {
                    if(!e) continue;
                    if(e->as<node>()->has(s))
                        return true;
                }
            }
            return false;
        }
        string to_string() const override {
            if (get("val"))
                return get("val")->to_string();
            else if(get("matched"))
            {
                return get("tip")->as<String>()->get_val() + get("matched")->to_string();
            }
            else
                return "EMPTY";
        }
    };


}
#endif //PROJECT_NODE_H
