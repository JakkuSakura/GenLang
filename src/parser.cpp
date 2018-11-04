#include "Basic.h"
#include "GC.h"
#include "Scanner.h"
using namespace GenLang;
#define new(type, args...) gc.newDynamicType<type>(args)

static GC gc;
struct Node : public Object {
    Node(const std::string &type) {
        setClassName(type);
    }
};
struct Expr : public Node {
    Expr() : Node("Expr") {}
};
struct Stmt : public Node {
    Stmt(Expr *expr) : Node("Stmt") {
        append("expr", expr);
    }
};
struct StmtBlock : public Node {
    StmtBlock(List *lst) : Node("StmtBlock") {
        append("list", lst);
    }
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
    GC gc;
    Scanner scanner(gc, stdin);
    while(!feof(stdin))
    {
        try {
            std::cout << scanner.getToken()->toString() << std::endl;
        } catch(const char *msg) {
            std::cerr << msg << std::endl;
        }
    }
}
