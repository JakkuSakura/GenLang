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
    void setVal(DynamicType *obj) {
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
struct LetExpr : public Expr {
    List *lst;
    LetExpr() {
        setClassName("LetExpr");
        lst = new(List);
        setVal(lst);
    }
    void append(Token *id) {
        lst->append(id);
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
    std::vector<Token *> tokens;
    int index = 0;
    void advance() {
        ++index;
    }
    Token *getToken() {
        return tokens[index];
    }
    std::map<void *, int> backups;
    void backup(void *frame) {
        backups[frame] = index;
    }
    void restore(void *frame) {
        index = backups[frame];
    }
    bool eps() {
        return getToken()->type == Token::Type::TEOF;
    }
    Token *oper(const std::string &s) {
        if(getToken()->type == Token::Type::OPERATOR
                && as(String, getTokenVal())->getVal() == s) {
            Token *tk = getToken();
            advance();
            return tk;
        }
        return NULL;
    }
    bool semi() {
        return oper(";");
    }
    Token *number() {
        if( getToken()->type == Token::Type::CONSTANT ) {
            Token *tk = getToken();
            advance();
            return tk;
        }
        return NULL;
    }
    Token *string() {
        if( getToken()->type == Token::Type::STRING ) {
            Token *tk = getToken();
            advance();
            return tk;
        }
        return NULL;
    }

    Object *getTokenVal() {
        return getToken()->getVal();
    }
    void readinTokens() {
        tokens.clear();
        index = 0;
        Token *tk;
        do {
            tk = scanner.getToken();
            tokens.push_back(tk);
            std::cout << tk->toString() << std::endl;
        } while (tk->type != Token::Type::TEOF);
    }
    Expr *numberExpr() {
        if(Token *tk = number()) {
            Expr *expr = new(Expr, tk->getVal());
            return expr;
        }
        return NULL;
    }
    Expr *multiExpr() {
        Expr *expr = 0;
        backup(&expr);
        if((expr = numberExpr())) {
            Token *tk;
            do {
                tk = 0;
                backup(&tk);
                if((tk = oper("*")) || (tk = oper("/")) || (tk = oper("%"))) {
                    if(Expr *expr2 = numberExpr())
                        expr = new(BinaryOperator, as(String, tk->getVal()), expr, expr2);
                    else
                        restore(&tk);
                }
            }while(tk);
            return expr;
        }
        restore(&expr);
        if((expr = numberExpr())) {
            return expr;
        }
        restore(&expr);
        return NULL;
    }
    #define d() std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    Expr *addExpr() {
        Expr *expr = 0;
        backup(&expr);
        if((expr = multiExpr())) {
            Token *tk;
            do {
                tk = 0;
                backup(&tk);
                if((tk = oper("+")) || (tk = oper("-"))) {
                    if(Expr *expr2 = multiExpr())
                        expr = new(BinaryOperator, as(String, tk->getVal()), expr, expr2);
                    else
                        restore(&tk);
                }
            }while(tk);
            return expr;
        }
        restore(&expr);
        return NULL;
    }
    LetExpr *letExpr() {
        if(as(String, getTokenVal())->getVal() == "let") {
            advance();
            Token *tk = 0;
            backup(&tk);
            if((tk = getToken())) {
                if(tk->getTokenType() == Token::Type::IDENTIFIER) {
                    LetExpr *le = new(LetExpr);
                    le->append(tk);
                    return le;
                }
            }
            restore(&tk);
        }
        return NULL;
    }
    Stmt *statement() {
        Stmt *s = 0;
        backup(&s);
        if(Expr *expr = addExpr()) {
            if(semi()) {
                s = new(Stmt, expr);
                return s;
            }
        }
        restore(&s);
        if(LetExpr *expr = letExpr()) {
            if(semi()) {
                s = new(Stmt, expr);
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
