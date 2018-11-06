#include "Basic.h"
#include "GC.h"
#include "Scanner.h"
using namespace GenLang;
#define new(type, args...) gc.newDynamicType<type>(args)
#define as(type, arg) ((type *)(arg))
#define d() std::cerr << __FILE__ << " " << __LINE__ << std::endl;
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
    Expr(DynamicType *dt) : Node("Expr") {
        setVal(dt);
    }
};
struct AssignExpr : public Expr {
    AssignExpr(String *id, Expr *expr) {
        setClassName("AssignExpr");
        append("id", id);
        setVal(expr);
    }
};
struct LetExpr : public Expr {
    List *lst;
    LetExpr() {
        setClassName("LetExpr");
        lst = new(List);
        setVal(lst);
    }
    void append(AssignExpr *as_expr) {
        lst->append(as_expr);
    }
    virtual std::string toString() const{
        return lst->toString();
    }
};

struct Stmt : public Node {
    Stmt() : Node("Stmt") { }
    Stmt(Expr *expr) : Node("Stmt") {
        append("expr", expr);
    }
    virtual std::string toString() const {
        Expr *e = as(Expr, get("expr"));
        return e ? e->toString() : "NULL";
    }
};
struct StmtBlock : public Stmt {
    List *list;
    StmtBlock(List *lst) {
        setClassName("StmtBlock");
        list = lst;
        append("list", lst);
    }
    virtual std::string toString() const {
        return getClassName() + list->toString();
    }
};
struct TypeKeyPair : public Node {
    TypeKeyPair(String *type, String *key) : Node("TypeKeyPair") {
        append("type", type);
        append("key", key);
    }
};
struct FunDeclExpr : public Expr {
    List *arguments;
    StmtBlock *body;
    FunDeclExpr() {
        setClassName("FunDeclExpr");
        arguments = new(List);
        append("arguments", arguments);
    }
    void addArgument(TypeKeyPair *arg) {
        arguments->append(arg);
    }
    void setBody(StmtBlock *blk) {
        body = blk;
        append("bode", blk);
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
    bool comma() {
        return oper(",");
    }
    bool semi() {
        return oper(";");
    }
    Token *number() {
        Token *tk = getToken();
        if( tk->type == Token::Type::CONSTANT || tk->type == Token::Type::IDENTIFIER ) {
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
        if((expr = numberExpr())) {
            Token *tk;
            do {
                tk = 0;
                if((tk = oper("*")) || (tk = oper("/")) || (tk = oper("%"))) {
                    if(Expr *expr2 = numberExpr())
                        expr = new(BinaryOperator, as(String, tk->getVal()), expr, expr2);
                    else
                        throw "except expression";
                }
            }while(tk);
            return expr;
        }
        return NULL;
    }
    Expr *addExpr() {
        if(Expr *expr = multiExpr()) {
            Token *tk;
            do {
                tk = 0;
                if((tk = oper("+")) || (tk = oper("-"))) {
                    if(Expr *expr2 = multiExpr())
                        expr = new(BinaryOperator, as(String, tk->getVal()), expr, expr2);
                    else
                        throw "except expression";
                }
            }while(tk);
            return expr;
        }
        return NULL;
    }
    LetExpr *letExpr() {
        if(as(String, getTokenVal())->getVal() == "let") {
            advance();
            LetExpr *le = new(LetExpr);
            while(1) {
                Token *tk = getToken();
                if(tk->getTokenType() == Token::Type::IDENTIFIER) {
                    advance();
                    Expr *nd = new(Expr);
                    if(oper("=")) {
                        if(!(nd = addExpr()))
                            throw "expected expression";
                    }

                    AssignExpr *ae = new(AssignExpr, as(String, tk->getVal()), nd);
                    le->append(ae);
                    if (!comma()) {
                        break;
                    }
                } else {
                    throw "must be an IDENTIFIER";
                }
            }
            return le;
        }
        return NULL;
    }
    StmtBlock *block() {
        if(oper("{")) {
            List *pgms = new(List);
            while(!oper("}")) {
                Stmt *st = statement();
                if(st)
                    pgms->append(st);
                else
                    throw "expected statement";
            }
            return new(StmtBlock, pgms);
        }
        return NULL;
    }
    FunDeclExpr *funDeclExpr() {
        //TODO
        return NULL;
    }
    Expr *expression() {
        if(Expr *expr = addExpr())
            return expr;
        if(Expr *expr = letExpr())
            return expr;
        if(Expr *expr = funDeclExpr())
            return expr;
        throw "excepted expression";
    }
    Stmt *statement() {
        if(eps())
            return NULL;
        if(StmtBlock *sb = block()) {
            return sb;
        }
        if(Expr *expr = expression()) {
            if(semi()) {
                Stmt *s = new(Stmt, expr);
                return s;
            }
        }
        throw "excepted statement";
    }

    List *program() {
        List *pgms = new(List);
        while(Stmt *st = statement()) {
            pgms->append(st);
        }
        return pgms;
    }
    List *parse() {
        readinTokens();
        std::cout << "Read token" << std::endl;
        return program();
    }

};
int main()
{
    GC gc;
    Scanner scanner(gc, stdin);
    Parser parser(gc, scanner);

    try{
        List *program = parser.parse();
        std::cout << program->toString() << std::endl;
    }catch(const char *s) {
        std::cerr << s << std::endl;
    }
}
