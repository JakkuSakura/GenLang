#include <bits/stdc++.h>
#include "Basic.h"
#include "GC.h"
using namespace GenLang;

#define new(type, args...) gc.newDynamicType<type>(args)
GC gc;
struct Node : public Object{
    Node(const std::string &type) {
        setClassName(type);
    }
};
struct Expr : public Node{
    Expr() : Node("Expr"){}
};
struct BinaryOperator : public Expr {
    BinaryOperator(String *type, Expr *left, Expr *right) {
        setUnorded(true);
        setClassName("Operator");
        append("type", type);
        append("left", left);
        append("right", right);
    }
};
struct SingleOperator : public Expr {
    SingleOperator(String *type, Expr *right) {
        setUnorded(true);
        setClassName("SingleOperator");
        append("type", type);
        append("right", right);
    }
};
int main()
{
    BinaryOperator *b = new(BinaryOperator, new(String, "+"), new(Expr), new(Expr));
    std::cout << b->toString() << std::endl;
}
