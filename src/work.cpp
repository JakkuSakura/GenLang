#include <iostream>
#include <cstring>
#include "node.h"
void print(const Node *node, int t)
{
    printf("%*s%s %s\n", t * 2, "", getNodeName(node->getNodeType()), node->toString());
    if(std::strlen(node->toString())) return;
    switch (node->getNodeType()) {
    case nBlock:
    {
        NBlock *n = (NBlock *)node;
        const StatementList &st = n->statements;
        for (size_t i = 0; i < st.size(); i++) {
            print(st[i], t + 1);
        }
        break;
    }

    case nExternDeclaration:
    {
        NExternDeclaration *n = (NExternDeclaration *)node;
        print(&n->type, t + 1);
        print(&n->id, t + 1);
        break;
    }
    case nVariableDeclaration:
    {
        NVariableDeclaration *n = (NVariableDeclaration *)node;
        print(&n->type, t + 1);
        print(&n->id, t + 1);
        NExpression *expr = n->assignmentExpr;
        if(expr) print(expr, t + 1);
        break;
    }
    case nFunctionDeclaration:
    {
        NFunctionDeclaration *n = (NFunctionDeclaration *)node;
        print(&n->type, t + 1);
        print(&n->id, t + 1);
        VariableList &arg = n->arguments;
        for (size_t i = 0; i < arg.size(); i++) {
            print(arg[i], t + 1);
        }
        NBlock &block = n->block;
        print(&block, t + 1);
        break;
    }
    case nExpressionStatement:
    {
        NExpression *ne = &((NExpressionStatement *)node)->expression;
        print(ne, t + 1);
        break;
    }
    case nBinaryOperator:
    {
        NBinaryOperator *n = (NBinaryOperator *)node;
        printf("%*soperator %d\n", t * 2 ,"", n->op);
        print(&n->lhs, t + 1);
        print(&n->rhs, t + 1);
        break;
    }
    case nReturnStatement:
    {
        NReturnStatement *n = (NReturnStatement *)node;
        print(&n->expression, t + 1);
        break;
    }
    case nMethodCall:
    {
        NMethodCall *n = (NMethodCall *)node;
        print(&n->id, t + 1);
        for (size_t i = 0; i < n->arguments.size(); i++) {
            print(n->arguments[i], t + 1);
        }
        break;
    }
    }
}
