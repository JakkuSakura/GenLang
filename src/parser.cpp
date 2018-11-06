#include "Basic.h"
#include "GC.h"
#include "Scanner.h"
using namespace GenLang;
#define alloc(type, args...) gc.newDynamicType<type>(args)
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
        lst = alloc(List);
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
    TypeKeyPair *type_name_pair;
    List *arguments;
    StmtBlock *body;
    FunDeclExpr(TypeKeyPair *tkp) {
        setClassName("FunDeclExpr");
        arguments = alloc(List);
        type_name_pair = tkp;
        append("arguments", arguments);
        append("typekeypair", tkp);
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
            Expr *expr = alloc(Expr, tk->getVal());
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
                        expr = alloc(BinaryOperator, as(String, tk->getVal()), expr, expr2);
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
                        expr = alloc(BinaryOperator, as(String, tk->getVal()), expr, expr2);
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
            LetExpr *le = alloc(LetExpr);
            while(1) {
                Token *tk = getToken();
                if(tk->getTokenType() == Token::Type::IDENTIFIER) {
                    advance();
                    Expr *nd = alloc(Expr);
                    if(oper("=")) {
                        if(!(nd = addExpr()))
                            throw "expected expression";
                    }

                    AssignExpr *ae = alloc(AssignExpr, as(String, tk->getVal()), nd);
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
            List *pgms = alloc(List);
            while(!oper("}")) {
                Stmt *st = statement();
                if(st)
                    pgms->append(st);
                else
                    throw "expected statement";
            }
            return alloc(StmtBlock, pgms);
        }
        return NULL;
    }
    String *getTypeName() {
        Token *tk = getToken();
        if(tk->type == Token::Type::TYPENAME) {
            advance();
            return as(String, tk->getVal());
        }
        return NULL;
    }
    String *getIdentifier(){
        Token *nm = getToken();
        if(nm->type == Token::Type::IDENTIFIER)  {
            advance();
            return as(String, nm->getVal());
        }
        return NULL;
    }
    TypeKeyPair *getPair() {
        if(String *tn = getTypeName()) {
            if(String *nm = getIdentifier())  {
                TypeKeyPair *tkp = alloc(TypeKeyPair, tn, nm);
                return tkp;
            } else {
                throw "excepted identifier";
            }
        }
        return NULL;
    }
    FunDeclExpr *funDeclExpr() {
        if(getToken()->type == Token::Type::TYPENAME) {
            TypeKeyPair *tkp =  getPair();
            FunDeclExpr *fexpr = alloc(FunDeclExpr, tkp);
            if(oper("(")) {
                while(!oper(")")){
                    TypeKeyPair *arg = getPair();
                    fexpr->addArgument(arg);
                }
            }else {
               throw "excepted '('";
            }
            StmtBlock *sb = block();
            if(!sb) {
                throw "excepted statement block";
            }
            fexpr->setBody(sb);
            return fexpr;
        }
        return NULL;
    }
    Expr *expression() {
        if(Expr *expr = addExpr())
            return expr;
        if(Expr *expr = letExpr())
            return expr;
        throw "excepted expression";
    }
    Stmt *statement() {
        if(eps())
            return NULL;
        if(StmtBlock *sb = block()) {
            return sb;
        }
        if(Expr *expr = funDeclExpr())
            return alloc(Stmt, expr);

        if(Expr *expr = expression()) {
            if(semi()) {
                Stmt *s = alloc(Stmt, expr);
                return s;
            }
        }
        throw "excepted statement";
    }

    List *program() {
        List *pgms = alloc(List);
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
// TODO oper '='

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
