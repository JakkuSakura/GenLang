#include <iostream>
#include <cstring>
#include "node.h"
void print(const Node *node, int t)
{
    printf("%*s%s %s\n", t * 2, "", getNodeName(node->getNodeType()), node->toString());
    if(std::strlen(node->toString())) return;
    switch (node->getNodeType()) {
    case nProgram:
        node = ((NProgram *)node)->block;
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
        printf("%*soperator %d\n", t * 2 + 2 ,"", n->op);
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
    case nIfStatement:
    {
        NIfStatement *n = (NIfStatement *)node;
        print(&n->expr, t + 1);
        print(&n->stmt, t + 1);
        break;
    }
    case nLoopStatement:
    {
        NLoopStatement *n = (NLoopStatement *)node;
        print(&n->init, t + 1);
        print(&n->judge, t + 1);
        print(&n->iter, t + 1);
        print(&n->stmt, t + 1);
        break;
    }
    }
}
void walk(const Node *node, void (*callback)(const Node *))
{
    callback(node);
    switch (node->getNodeType()) {
    case nProgram:
        node = ((NProgram *)node)->block;
    case nBlock:
    {
        NBlock *n = (NBlock *)node;
        const StatementList &st = n->statements;
        for (size_t i = 0; i < st.size(); i++) {
            walk(st[i], callback);
        }
        break;
    }

    case nExternDeclaration:
    {
        NExternDeclaration *n = (NExternDeclaration *)node;
        walk(&n->type, callback);
        walk(&n->id, callback);
        break;
    }
    case nVariableDeclaration:
    {
        NVariableDeclaration *n = (NVariableDeclaration *)node;
        walk(&n->type, callback);
        walk(&n->id, callback);
        NExpression *expr = n->assignmentExpr;
        if(expr) walk(expr, callback);
        break;
    }
    case nFunctionDeclaration:
    {
        NFunctionDeclaration *n = (NFunctionDeclaration *)node;
        walk(&n->type, callback);
        walk(&n->id, callback);
        VariableList &arg = n->arguments;
        for (size_t i = 0; i < arg.size(); i++) {
            walk(arg[i], callback);
        }
        NBlock &block = n->block;
        walk(&block, callback);
        break;
    }
    case nExpressionStatement:
    {
        NExpression *ne = &((NExpressionStatement *)node)->expression;
        walk(ne, callback);
        break;
    }
    case nBinaryOperator:
    {
        NBinaryOperator *n = (NBinaryOperator *)node;
        printf("%*soperator %d\n", t * 2 + 2 ,"", n->op);
        walk(&n->lhs, callback);
        walk(&n->rhs, callback);
        break;
    }
    case nReturnStatement:
    {
        NReturnStatement *n = (NReturnStatement *)node;
        walk(&n->expression, callback);
        break;
    }
    case nMethodCall:
    {
        NMethodCall *n = (NMethodCall *)node;
        walk(&n->id, callback);
        for (size_t i = 0; i < n->arguments.size(); i++) {
            walk(n->arguments[i], callback);
        }
        break;
    }
    case nIfStatement:
    {
        NIfStatement *n = (NIfStatement *)node;
        walk(&n->expr, callback);
        walk(&n->stmt, callback);
        break;
    }
    case nLoopStatement:
    {
        NLoopStatement *n = (NLoopStatement *)node;
        walk(&n->init, callback);
        walk(&n->judge, callback);
        walk(&n->iter, callback);
        walk(&n->stmt, callback);
        break;
    }
    }
}
void pas_init(const NProgram *n)
{
    printf("this program is inited\n");
}

void pas_code(const NProgram *node)
{
    pas_init(node);
    pas_generate(node);

}
