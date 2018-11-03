#include "node.h"
using namespace nodetype;
const NFunctionDeclaration *now_function;
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
    case nImportDeclaration:
    {
        NImportDeclaration *n = (NImportDeclaration *)node;
        walk(&n->id, callback);
        break;
    }
    case nExternDeclaration:
    {
        NExternDeclaration *n = (NExternDeclaration *)node;
        walk(&n->type, callback);
        walk(&n->id, callback);
        break;
    }
    case nVariableDeclarationStatement:
    {
        node = &((NVariableDeclarationStatement *)node)->var_decl;
        walk(node, callback);
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
        now_function = n;
        walk(&n->type, callback);
        walk(&n->id, callback);
        VariableList &arg = n->arguments;
        for (size_t i = 0; i < arg.size(); i++) {
            walk(arg[i], callback);
        }
        NBlock &block = n->block;
        walk(&block, callback);
        now_function = NULL;
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
