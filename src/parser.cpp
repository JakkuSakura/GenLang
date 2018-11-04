#include "Basic.h"
#include "GC.h"
#include "Scanner.h"
using namespace GenLang;
#define new(type, args...) gc.newDynamicType<type>(args)
#define as(type, args...) ((type *)args)
static GC gc;
struct Node : public Object {
    Node(const std::string &type) {
        setClassName(type);
    }
    void setVal(Object *obj) {
        put("val", obj);
    }
};
struct Expr : public Node {
    Expr() : Node("Expr") {}
    template<class T>
    Expr(T *i) : Node("Expr") {
        setVal(i);
    }
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
struct Parser {
    GC &gc;
    Scanner &scanner;
    Parser(GC &gc, Scanner &sc) : gc(gc), scanner(sc) { }
    std::vector<Token *> v;
    int index = 0;
    void advance() {
        ++index;
    }
    std::map<void *, int> backups;
    void backup(void *frame) {
        backups[frame] = index;
    }
    void restore(void *frame) {
        index = backups[frame];
    }
    bool eps() {
        return v[index]->type == Token::Type::TEOF;
    }
    bool semi() {
        return v[index]->type == Token::Type::OPERATOR
            && as(String, getTokenVal())->getVal() == ";";
    }
    bool number() {
        return v[index]->type == Token::Type::CONSTANT;
    }
    bool string() {
        return v[index]->type == Token::Type::STRING;
    }
    Token *oper() {
        if(v[index]->type == Token::Type::OPERATOR)
            return v[index];
        return NULL;
    }

    Object *getTokenVal() {
        return v[index]->getVal();
    }
    void readinTokens() {
        v.clear();
        index = 0;
        Token *tk;
        do
        {
            tk = scanner.getToken();
            v.push_back(tk);
            std::cout << tk->toString() << std::endl;
        }while (tk->type != Token::Type::TEOF);
    }
    Expr *exprElement() {
        Expr *expr = 0;
        backup(&expr);
        if(number() || string()) {
            expr = new(Expr, getTokenVal());
            advance();
            return expr;
        }
        restore(&expr);
        return NULL;
    }
    Expr *expression(){
        Expr *expr = 0;
        backup(&expr);
        if((expr = exprElement())) {
            Token *tk = 0;
            if((tk = oper()))
            {
                advance();
                Expr *expr2 = 0;
                backup(&expr2);
                expr2 = expression();
                if(expr2)
                    return new(BinaryOperator, as(String, tk->getVal()), expr, expr2);
                restore(&expr2);
            } else {
                return expr;
            }
        }
        restore(&expr);
        return NULL;
    }
    Stmt *statement() {
        Stmt *s = 0;
        backup(&s);
        if(Expr *expr = expression()) {
            if(semi()) {
                s = new(Stmt, expr);
                advance();
                return s;
            }
        }
        restore(&s);
        return NULL;
    }

    List *program() {
        List *pgms = new(List);
        while(Stmt *st = statement()) {
            pgms->append(st);
        }
        return pgms;
    }
    void parse() {
        readinTokens();
        std::cout << "Read token" << std::endl;
        backups.clear();
        std::cout << program()->toString() << std::endl;
    }

};
int main()
{
    GC gc;
    Scanner scanner(gc, stdin);
    Parser parser(gc, scanner);
    parser.parse();
}
