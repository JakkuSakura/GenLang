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
};

struct Expr : public Node {
    Expr() : Node("Expr") {}

    Expr(DynamicType *dt) : Node("Expr") {
        setVal(dt);
    }

    void setVal(DynamicType *obj) {
        put("val", obj);
    }

    DynamicType *getVal() {
        return get("val");
    }

    const DynamicType *getVal() const {
        return get("val");
    }
};

struct AssignExpr : public Expr {
    AssignExpr(String *id, Expr *expr) {
        setClassName("AssignExpr");
        append("id", id);
        setVal(expr);
    }

    virtual std::string toString() const {
        return as(String, get("id"))->getVal() + "=" + as(Expr, getVal())->toString();
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

    virtual std::string toString() const {
        return getClassName() + lst->toString();
    }
};

struct FuncCall : public Expr {
    FuncCall(String *id, List *arguments) {
        setClassName("FuncCall");
        put("id", id);
        put("arguments", arguments);
    }
};

struct Stmt : public Node {
    Stmt() : Node("Stmt") {}

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
        setUnorded(true);
        append("type", type);
        append("key", key);
    }

    const std::string &getName() const {
        return as(String, get("key"))->getVal();
    }
};

struct FunDeclExpr : public Expr {
    TypeKeyPair *type_name_pair;
    List *arguments;
    StmtBlock *body;

    FunDeclExpr(TypeKeyPair *tkp) {
        setClassName("FunDeclExpr");
        setUnorded(true);
        type_name_pair = tkp;
        append("typekeypair", tkp);
        arguments = alloc(List);
        append("arguments", arguments);
    }

    void addArgument(TypeKeyPair *arg) {
        arguments->append(arg);
    }

    void setBody(StmtBlock *blk) {
        body = blk;
        append("body", blk);
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

    Parser(GC &gc, Scanner &sc) : gc(gc), scanner(sc) {}

    std::vector<Token *> tokens;
    std::map<std::string, FunDeclExpr *> functions;
    std::set<std::string> variables;

    int index = 0;

    void advance() {
        ++index;
    }

    void back() {
        --index;
    }

    Token *getToken() {
        return tokens[index];
    }

    bool eps() {
        return getToken()->type == Token::Type::TEOF;
    }

    Token *oper(const std::string &s) {
        if (getToken()->type == Token::Type::OPERATOR
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
        if (tk->type == Token::Type::CONSTANT) {
            advance();
            return tk;
        }
        return NULL;
    }

    Token *string() {
        Token *tk = getToken();
        if (tk->type == Token::Type::STRING) {
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
        if (Token *tk = number()) {
            Expr *expr = alloc(Expr, tk->getVal());
            return expr;
        }
        return NULL;
    }

    FuncCall *funcCall() {
        if (String *id = getIdentifier()) {
            if (testFunc(id->getVal()))
                if (oper("(")) {
                    List *arguments = alloc(List);
                    while (!oper(")")) {
                        if (Expr *expr = expression()) {
                            arguments->append(expr);
                            comma();
                        } else
                            throw "excepted expression";
                    }
                    return alloc(FuncCall, id, arguments);
                }
            back();
            return NULL;
        }
        return NULL;
    }

    Expr *multiExpr() {
        Expr *expr = 0;
        if ((expr = numberExpr()) || (expr = funcCall()) || (expr = parentheses())) {
            Token *tk;
            do {
                tk = 0;
                if ((tk = oper("*")) || (tk = oper("/")) || (tk = oper("%"))) {
                    if (Expr *expr2 = numberExpr())
                        expr = alloc(BinaryOperator, as(String, tk->getVal()), expr, expr2);
                    else
                        throw "except expression";
                }
            } while (tk);
            return expr;
        }
        return NULL;
    }

    Expr *addExpr() {
        if (Expr *expr = multiExpr()) {
            Token *tk;
            do {
                tk = 0;
                if ((tk = oper("+")) || (tk = oper("-"))) {
                    if (Expr *expr2 = multiExpr())
                        expr = alloc(BinaryOperator, as(String, tk->getVal()), expr, expr2);
                    else
                        throw "except expression";
                }
            } while (tk);
            return expr;
        }
        return NULL;
    }

    AssignExpr *assignExpr() {
        if (String *id = getIdentifier()) {
            if (oper("=")) {
                if (Expr *nd = expression())
                    return alloc(AssignExpr, as(String, id), nd);

                else
                    throw "expected expression";
            } else {
                back();
                return NULL;
            }
        }
        return NULL;
    }

    bool testFunc(const std::string &id) {
        return functions.find(id) != functions.end();
    }

    bool testVar(const std::string &id) {
        return variables.count(id) > 0;
    }

    void addVar(const std::string &id) {
        variables.insert(id);
    }

    Expr *parentheses() {
        if (oper("(")) {
            Expr *expr = expression();
            if (oper(")")) {
                return expr;
            } else {
                throw "excepted ')'";
            }
        }
        return NULL;
    }

    LetExpr *letExpr() {
        if (as(String, getTokenVal())->getVal() == "let") {
            advance();
            LetExpr *le = alloc(LetExpr);
            while (1) {
                if (String *id = getIdentifier()) {
                    Expr *nd = alloc(Expr);
                    if (oper("=")) {
                        if (!(nd = addExpr()))
                            throw "expected expression";
                    }
                    auto ae = alloc(AssignExpr, id, nd);
                    le->append(ae);
                    if (testVar(id->getVal()))
                        throw "do not declare a variable twice";
                    if (testFunc(id->getVal()))
                        throw "it is already a function name";

                    addVar(id->getVal());


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
        if (oper("{")) {
            List *pgms = alloc(List);
            while (!oper("}")) {
                Stmt *st = statement();
                if (st)
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
        if (tk->type == Token::Type::TYPENAME) {
            advance();
            return as(String, tk->getVal());
        }
        return NULL;
    }

    String *getIdentifier() {
        Token *nm = getToken();
        if (nm->type == Token::Type::IDENTIFIER) {
            advance();
            return as(String, nm->getVal());
        }
        return NULL;
    }

    TypeKeyPair *getPair() {
        if (String *tn = getTypeName()) {
            if (String *nm = getIdentifier()) {
                TypeKeyPair *tkp = alloc(TypeKeyPair, tn, nm);
                return tkp;
            } else {
                throw "excepted identifier";
            }
        }
        return NULL;
    }

    FunDeclExpr *funDeclExpr() {
        if (TypeKeyPair *tkp = getPair()) {
            if (testFunc(tkp->getName()))
                throw "do not declare a function for twice";
            if (testVar(tkp->getName()))
                throw "it it already a variable name";
            FunDeclExpr *fexpr = alloc(FunDeclExpr, tkp);
            if (oper("(")) {
                while (1) {
                    if (TypeKeyPair *arg = getPair()) {
                        fexpr->addArgument(arg);
                        if(!comma() && !oper(")") ) {
                            throw "excepted ')'";
                        }
                    } else
                        throw "excepted Type Key pair(s)";
                }
            } else {
                throw "excepted '('";
            }
            functions[tkp->getName()] = fexpr;

            StmtBlock *sb = block();
            if (!sb) {
                throw "excepted statement block";
            }
            fexpr->setBody(sb);
            return fexpr;
        }
        return NULL;
    }

    Expr *expression() {
        if (Expr *expr = assignExpr())
            return expr;

        if (Expr *expr = addExpr())
            return expr;
        throw "excepted expression";
    }

    Stmt *statement() {
        if (eps())
            return NULL;
        if (StmtBlock *sb = block()) {
            return sb;
        }

        if (Expr *expr = letExpr())
        {
            if (semi()) {
                Stmt *s = alloc(Stmt, expr);
                return s;
            }
        }

        if (Expr *expr = funDeclExpr())
            return alloc(Stmt, expr);

        if (Expr *expr = expression()) {

            if (semi()) {
                Stmt *s = alloc(Stmt, expr);
                return s;
            }
        }
        throw "excepted statement";
    }

    List *program() {
        List *pgms = alloc(List);
        try {
            while (Stmt *st = statement()) {
                pgms->append(st);
                d();
                std::cout << st->toString() << std::endl;
            }
        } catch (const char *s) {
            std::cerr << s << std::endl;
        }
        return pgms;
    }

    List *parse() {
        readinTokens();
        std::cout << "Read token" << std::endl;
        return program();
    }
};

int main() {
    GC gc;
    Scanner scanner(gc, stdin);
    Parser parser(gc, scanner);

    try {
        List *program = parser.parse();
        std::cout << program << std::endl;
    } catch (const char *s) {
        std::cerr << s << std::endl;
    }
}