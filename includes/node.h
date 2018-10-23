#ifndef __GENLANG_NODE_H
#define __GENLANG_NODE_H
#include <iostream>
#include <vector>
#include <cstdio>
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;
namespace nodetype{
    enum  {node,nExpression,nStatement,nInteger,nDouble,nIdentifier, nVarType,
               nMethodCall,nBinaryOperator,nAssignment,nBlock,nExpressionStatement,
               nReturnStatement,nVariableDeclaration,nExternDeclaration,
                nFunctionDeclaration, nIfStatement, nLoopStatement, nString, nProgram,
                nVariableDeclarationStatement
              };
};
const char * getNodeName(int t);
class Node {
public:
    int m_type;
    void setNodeType(int tp)
    {
        m_type = tp;
    }
    int getNodeType() const
    {
        return m_type;
    }
    virtual const char *toString() const
    {
        return "";
    }
    virtual ~Node() {}
};


class NExpression : public Node {
public:
    NExpression() {
        setNodeType(nodetype::nExpression);
    }
};

class NStatement : public Node {
public:
    NStatement() {
        setNodeType(nodetype::nStatement);
    }
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) {
        setNodeType(nodetype::nInteger);
    }
    virtual const char *toString() const {
        static char buf[24];
        std::sprintf(buf, "%lld", value);
        return buf;
    }
};

class NDouble : public NExpression {
public:
    double value;

    NDouble(double value) : value(value) {
        setNodeType(nodetype::nDouble);
    }
    virtual const char *toString() const {
        static char buf[24];
        std::sprintf(buf, "%g", value);
        return buf;
    }
};

class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string &name) : name(name) {
        setNodeType(nodetype::nIdentifier);
    }
    virtual const char *toString() const
    {
        return name.c_str();
    }
};
class NVarType : public NIdentifier {
public:
    NVarType(const std::string &name) : NIdentifier(name) {
        setNodeType(nodetype::nVarType);
    }
};


class NMethodCall : public NExpression {
public:
    const NIdentifier &id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier &id, ExpressionList &arguments) :
        id(id), arguments(arguments) {
        setNodeType(nodetype::nMethodCall);
    }
    NMethodCall(const NIdentifier &id) :  id(id) {
        setNodeType(nodetype::nMethodCall);
    }

};

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression &lhs;
    NExpression &rhs;
    NBinaryOperator(NExpression &lhs, int op, NExpression &rhs) :
        lhs(lhs), rhs(rhs), op(op) {
        setNodeType(nodetype::nBinaryOperator);
    }

};

class NBlock : public NStatement {
public:
    StatementList statements;
    NBlock() {
        setNodeType(nodetype::nBlock);
    }

};

class NExpressionStatement : public NStatement {
public:
    NExpression &expression;
    NExpressionStatement(NExpression &expression) :
        expression(expression) {
        setNodeType(nodetype::nExpressionStatement);
    }

};

class NReturnStatement : public NStatement {
public:
    NExpression &expression;
    NReturnStatement(NExpression &expression) :
        expression(expression) {
        setNodeType(nodetype::nReturnStatement);
    }

};

class NVariableDeclaration : public Node {
public:
    const NIdentifier &type;
    NIdentifier &id;
    NExpression *assignmentExpr;
    NVariableDeclaration(const NIdentifier &type, NIdentifier &id) :
        type(type), id(id) {
        setNodeType(nodetype::nVariableDeclaration);
        assignmentExpr = NULL;
    }
    NVariableDeclaration(const NIdentifier &type, NIdentifier &id, NExpression *assignmentExpr) :
        type(type), id(id), assignmentExpr(assignmentExpr) {
        setNodeType(nodetype::nVariableDeclaration);
    }
};
class NVariableDeclarationStatement : public NStatement
{
public:
    const NVariableDeclaration &var_decl;
    NVariableDeclarationStatement(const NVariableDeclaration &var_decl) :
        var_decl(var_decl) {
        setNodeType(nodetype::nVariableDeclarationStatement);
    }

};
class NIfStatement : public NStatement {
public:
    const NExpression &expr;
    const NStatement &stmt;
    NIfStatement(const NExpression &expr, const NStatement &stmt) :
        expr(expr), stmt(stmt) {
        setNodeType(nodetype::nIfStatement);
    }
};
class NLoopStatement : public NStatement {
public:
    const Node &init;
    const NExpression &judge;
    const NExpression &iter;
    const NStatement &stmt;
    NLoopStatement(const Node &init, const NExpression &judge, const NExpression &iter, const NStatement &stmt) :
        init(init), judge(judge), iter(iter), stmt(stmt) {
        setNodeType(nodetype::nLoopStatement);
    }
};
class NExternDeclaration : public NStatement {
public:
    const NIdentifier &type;
    const NIdentifier &id;
    VariableList arguments;
    NExternDeclaration(const NIdentifier &type, const NIdentifier &id,
                       const VariableList &arguments) :
        type(type), id(id), arguments(arguments) {
        setNodeType(nodetype::nExternDeclaration);
    }

};

class NFunctionDeclaration : public NStatement {
public:
    const NIdentifier &type;
    const NIdentifier &id;
    VariableList arguments;
    NBlock &block;
    NFunctionDeclaration(const NIdentifier &type, const NIdentifier &id,
                         const VariableList &arguments, NBlock &block) :
        type(type), id(id), arguments(arguments), block(block) {
        setNodeType(nodetype::nFunctionDeclaration);
    }

};
class NString : public NExpression
{
public:
    std::string value;
    NString(const std::string &s):value(s)
    {
        setNodeType(nodetype::nString);
    }
    virtual const char *toString() const
    {
        return value.c_str();
    }

};
class NProgram : public Node {
public:
    NBlock *block;
    NProgram(){
        block = NULL;
        setNodeType(nodetype::nProgram);
    }

    void setBlock(NBlock *b)
    {
            block = b;
    }
};

#endif
