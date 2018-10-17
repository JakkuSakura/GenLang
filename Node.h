#include <iostream>
#include <vector>
typedef Node *YYSTYPE;
class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;
typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;
class Node
{
public:
    virtual ~Node() {}
};

class NExpression : public Node
{

};

class NStatement : public Node
{

};

class NReserved : public Node
{
public:
    int token;
    NReserved(int tk) : token(tk) {}
};

class NInteger : public NExpression
{
public:
    int size;
    int is_signed;
    __int64 value;
    NInteger(__int64 value, int size, int is_signed)
    : value(value), size(size), is_signed(is_signed) { }

};

class NDouble : public NExpression
{
public:
    double value;
    NDouble(double value) : value(value) { }

};
class NString : public NExpression
{
public:
    std::string value;
    NString(const char *str) : value(str){}
}

class NIdentifier : public NExpression
{
public:
    std::string name;
    NIdentifier(const std::string &name) : name(name) { }

};

class NMethodCall : public NExpression
{
public:
    const NIdentifier *id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier &id, ExpressionList &arguments) :
        id(&id), arguments(arguments) { }
    NMethodCall(const NIdentifier &id) : id(&id) { }

};
class NBinaryOperator : public NExpression
{
public:
    int op;
    NExpression *lhs;
    NExpression *rhs;
    NBinaryOperator(NExpression &lhs, int op, NExpression &rhs) :
        lhs(&lhs), rhs(&rhs), op(op) { }
    NBinaryOperator(int op) :
            lhs(0), rhs(0), op(op) { }

};
class NSingleOperator : public NExpression
{
public:
    int op;
    NExpression *expr;
    NBinaryOperator(int op, NExpression &e):
        expr(&e), op(op) { }

};
class NMultiOperator : public Node // it's not a NExpression
{
    int op;
    NMultiOperator(int op) : op(op) {}
}
class NBlock : public NExpression
{
public:
    StatementList statements;
    NBlock() { }

};

class NExpressionStatement : public NStatement
{
public:
    NExpression *expression;
    NExpressionStatement(NExpression &expression) :
        expression(expression) { }

};

class NVariableDeclaration : public NStatement
{
public:
    const NIdentifier *type;
    NIdentifier *id;
    NExpression *assignmentExpr;
    NVariableDeclaration(const NIdentifier &type, NIdentifier &id) :
        type(&type), id(&id) { }
    NVariableDeclaration(const NIdentifier &type, NIdentifier &id, NExpression &assignmentExpr) :
        type(&type), id(&id), assignmentExpr(&assignmentExpr) { }

};

class NFunctionDeclaration : public NStatement
{
public:
    const NIdentifier *type;
    const NIdentifier *id;
    VariableList arguments;
    NBlock *block;
    NFunctionDeclaration(const NIdentifier &type, const NIdentifier &id,
                         const VariableList &arguments, NBlock &block) :
        type(&type), id(&id), arguments(arguments), block(&block) { }
};
