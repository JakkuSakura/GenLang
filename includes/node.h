#ifndef __GENLANG_NODE_H
#define __GENLANG_NODE_H
#include <iostream>
#include <vector>

class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

enum nodetype {node,nExpression,nStatement,nInteger,nDouble,nIdentifier, nVarType,
               nMethodCall,nBinaryOperator,nAssignment,nBlock,nExpressionStatement,
               nReturnStatement,nVariableDeclaration,nExternDeclaration,nFunctionDeclaration
              };
const char * getNodeName(nodetype t);
class Node {
public:
    nodetype m_type;
    void setNodeType(nodetype tp)
    {
        m_type = tp;
    }
	nodetype getNodeType() const
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
    NExpression(){
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
        std::sprintf(buf, "%d", value);
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

class NAssignment : public NExpression {
public:
    NIdentifier &lhs;
    NExpression &rhs;
    NAssignment(NIdentifier &lhs, NExpression &rhs) :
        lhs(lhs), rhs(rhs) {
        setNodeType(nodetype::nAssignment);
    }

};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock(){
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

class NVariableDeclaration : public NStatement {
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

#endif
