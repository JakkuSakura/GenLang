#include <iostream>
#include <cstring>
#include <cstdlib>
#include "node.h"
using namespace std;
using namespace nodetype;
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
void pas_print_var_table(const NProgram *n)
{
    printf("var\n");
}
void pas_generate(const Node *node);
void pas_print_functions(const NProgram *node)
{   
    StatementList &st = node->block->statements;
    for(int i = 0; i < st.size(); ++i)
    {
        if( st[i]->getNodeType() ==  nFunctionDeclaration)
        {
            NFunctionDeclaration *n = (NFunctionDeclaration *)st[i];
            printf("function %s(", n->id.toString());
            VariableList &arg = n->arguments;
            for (size_t i = 0; i < arg.size(); i++) {
                if(i >= 1) printf(",");
                printf("%s:%s", arg[i]->id.toString(), arg[i]->type.toString());
            }
            printf("):%s\n", n->type.toString());

            NBlock &block = n->block;
            pas_generate(&block);
            const StatementList &st = block.statements;
            for (size_t i = 0; i < st.size(); i++) {
                printf("%s\n", getNodeName(st[i]->getNodeType()));
            }

            break;
        }
    }
    
}
void pas_generate(const Node *node)
{
    switch (node->getNodeType()) {
    case nProgram:
    {
        const NProgram *n = (NProgram *)node;
        printf("program %ld\n", time(0));
        pas_print_var_table(n);
        pas_print_functions(n);
        printf("begin\n");
        
        NBlock *no = n->block;
        const StatementList &st = no->statements;
        for (size_t i = 0; i < st.size(); i++) {
            pas_generate(st[i]);
        }
        printf("end.\n");
        break;
    }
    case nBlock:
    {
        printf("begin\n");
        NBlock *n = (NBlock *)node;
        const StatementList &st = n->statements;
        for (size_t i = 0; i < st.size(); i++) {
            pas_generate(st[i]);
        }
        printf("end;\n");
        break;
    }
    /* TODO: I dont know here
    case nExternDeclaration:
    {
        NExternDeclaration *n = (NExternDeclaration *)node;
        pas_generate(&n->type);
        pas_generate(&n->id);
        break;
    }*/
    case nVariableDeclaration:
    {
        NVariableDeclaration *n = (NVariableDeclaration *)node;
        pas_generate(&n->type);
        pas_generate(&n->id);
        Node *expr = n->assignmentExpr;
        if(expr) pas_generate(expr);
        break;
    }
    /*
    case nExpressionStatement:
    {
        NExpression *ne = &((NExpressionStatement *)node)->expression;
        pas_generate(ne);
        printf("(there's an expression);\n");
        break;
    }*/
    
    case nBinaryOperator:
    {
        NBinaryOperator *n = (NBinaryOperator *)node;
        pas_generate(&n->lhs);
        printf("(%d)", n->op);
        pas_generate(&n->rhs);
        break;
    }
    case nReturnStatement:
    {
        NReturnStatement *n = (NReturnStatement *)node;
        pas_generate(&n->expression);
        printf("%s\n", "functionID");
        break;
    }
    case nMethodCall:
    {
        NMethodCall *n = (NMethodCall *)node;
        pas_generate(&n->id);
        for (size_t i = 0; i < n->arguments.size(); i++) {
            pas_generate(n->arguments[i]);
        }
        break;
    }
    case nIfStatement:
    {
        NIfStatement *n = (NIfStatement *)node;
        pas_generate(&n->expr);
        pas_generate(&n->stmt);
        break;
    }
    case nLoopStatement:
    {
        NLoopStatement *n = (NLoopStatement *)node;
        pas_generate(&n->init);
        pas_generate(&n->judge);
        pas_generate(&n->iter);
        pas_generate(&n->stmt);
        break;
    }
    default:
    {
        printf("%s", node->toString());
    }
    }
}
void pas_code(const NProgram *node)
{
    pas_init(node);
    pas_generate(node);

}
