#include <iostream>
#include <cstring>
#include <cstdlib>
#include "utils.h"
#include "node.h"
#include "const_table.h"
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
    case nImportDeclaration:
    {
        NImportDeclaration *n = (NImportDeclaration *)node;
        print(&n->id, t + 1);
        break;
    }
    case nExternDeclaration:
    {
        NExternDeclaration *n = (NExternDeclaration *)node;
        print(&n->type, t + 1);
        print(&n->id, t + 1);
        break;
    }
    case nVariableDeclarationStatement:
        node = &((NVariableDeclarationStatement *)node)->var_decl;
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
static const NFunctionDeclaration *now_function;
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
extern const char *token_table_lang;
void pas_init(const NProgram *n)
{
    token_table_lang = "pascal";
}

const char *pas_get_pas_type(const char *t)
{
#define mapping(from, to)  if (strcmp(#from, t) == 0) return #to
    mapping(int,longint);
    mapping(double,real);
    mapping(string,string);
    return t;
#undef mapping
}
void pas_generate(const Node *node);


static VariableList pas_varlist;
static const NFunctionDeclaration *now_func_needed;
static bool enable_limit_find_vars;
void pas_find_vars(const Node *n)
{
    if(enable_limit_find_vars && now_function != now_func_needed) return;
    if(n->getNodeType() == nodetype::nVariableDeclaration)
    {
        pas_varlist.push_back((NVariableDeclaration *)n);
    }
}
void pas_generate_vars(const NBlock *func_blk, const NFunctionDeclaration *now_func = NULL, bool enable_limit = false)
{
    now_func_needed = now_func;
    enable_limit_find_vars = enable_limit;

    pas_varlist.clear();
    walk(func_blk, pas_find_vars);

    if(pas_varlist.size())
    {
        printf("var\n");
        for (size_t i = 0; i < pas_varlist.size(); i++) {
            const NVariableDeclaration &var_decl = *pas_varlist[i];
            printf("%s:%s;\n", var_decl.id.toString(), pas_get_pas_type(var_decl.type.toString()));
        }
    }

}

void pas_print_functions(const NProgram *node)
{
    StatementList &st = node->block->statements;
    for(int i = 0; i < st.size(); ++i)
    {
        if( st[i]->getNodeType() ==  nFunctionDeclaration)
        {
            NFunctionDeclaration *n = (NFunctionDeclaration *)st[i];
            bool is_void = equals(n->type.name.c_str(), "void");
            printf("%s %s(", !is_void ? "function" : "procedure", n->id.toString());
            VariableList &arg = n->arguments;
            for (size_t i = 0; i < arg.size(); i++) {
                if(i >= 1) printf(";");
                printf("%s:%s", arg[i]->id.toString(), pas_get_pas_type(arg[i]->type.toString()));
            }
            if(is_void) printf(");\n");
            else printf("):%s;\n", pas_get_pas_type(n->type.toString()));
            now_function = n;
            NBlock &block = n->block;
            pas_generate_vars(&block);
            pas_generate(&block);
            now_function = NULL;
        }
    }
}
static std::vector<NImportDeclaration *> pas_import_list;
void pas_find_imports(const Node *n)
{
    if(n->getNodeType() == nodetype::nImportDeclaration)
    {
        pas_import_list.push_back((NImportDeclaration *)n);
    }
}
void catfile(const char *filename)
{
    FILE *f = fopen(filename, "r");
    char buf[256];
    while (fgets(buf, 256, f), !feof(f)) {
        fprintf(stdout, "%s", buf);
    }
    fclose(f);
}
void pas_import_decl(const NProgram *node)
{
    pas_import_list.clear();
    walk(node, pas_find_imports);
    char buf[256];
    for (size_t i = 0; i < pas_import_list.size(); i++) {
        const NImportDeclaration &import_decl = *pas_import_list[i];
        sprintf(buf, "./gstl/pas/%s.imp", import_decl.id.toString());
        catfile(buf);
    }
}
void pas_generate(const Node *node)
{
    //printf("(%s)", getNodeName(node->getNodeType()));
    switch (node->getNodeType()) {
    case nProgram:
    {
        const NProgram *n = (NProgram *)node;
        printf("program prog%ld;\n", time(0));
        NBlock *no = n->block;
        pas_generate_vars(no, NULL, true);
        pas_import_decl(n);
        pas_print_functions(n);
        printf("begin\n");

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
        // pas_generate(&n->type);
        // printf(" ");
        pas_generate(&n->id);
        Node *expr = n->assignmentExpr;
        if(expr)
        {
            printf(" := ");
            pas_generate(expr);
        }
        break;
    }
    case nVariableDeclarationStatement:
    {
        NVariableDeclarationStatement *n = (NVariableDeclarationStatement *)node;
        pas_generate(&n->var_decl);
        printf(";\n");
        break;
    }
    case nIdentifier:
    {
        printf("%s", node->toString());
        break;
    }

    case nExpressionStatement:
    {
        NExpression *ne = &((NExpressionStatement *)node)->expression;
        pas_generate(ne);
        printf(";\n");
        break;
    }

    case nBinaryOperator:
    {
        NBinaryOperator *n = (NBinaryOperator *)node;
        pas_generate(&n->lhs);
        printf(" %s ", getTokenString(n->op));
        pas_generate(&n->rhs);
        break;
    }
    case nReturnStatement:
    {
        NReturnStatement *n = (NReturnStatement *)node;
        printf("%s := ", now_function->id.toString());
        pas_generate(&n->expression);
        printf(";\n");
        break;
    }
    case nMethodCall:
    {
        NMethodCall *n = (NMethodCall *)node;
        pas_generate(&n->id);
        if(n->arguments.size())
        {
            printf("(");
            for (size_t i = 0; i < n->arguments.size(); i++) {
                pas_generate(n->arguments[i]);
            }
            printf(")");
        }
        break;
    }
    case nIfStatement:
    {
        NIfStatement *n = (NIfStatement *)node;
        printf("if ");
        pas_generate(&n->expr);
        printf(" then\n");
        pas_generate(&n->stmt);
        break;
    }
    case nLoopStatement:
    {
        NLoopStatement *n = (NLoopStatement *)node;

        pas_generate(&n->init);
        printf(";\n");

        printf("while ");
        pas_generate(&n->judge);

        printf(" do\n");
        printf("begin\n");

        const NStatement &st = n->stmt;

        if(st.getNodeType() == nodetype::nStatement) pas_generate(&st);
        else {
            const StatementList &st2 = ((NBlock *)&st)->statements;
            for (size_t i = 0; i < st2.size(); i++) {
                pas_generate(st2[i]);
            }
        }

        pas_generate(&n->iter);
        printf(";\n");
        printf("end;\n");


        break;
    }
    case nString:
    {
        printf("\'%s\'", node->toString());
        break;
    }
    case nChar:
    {
        NChar *nch = (NChar *)node;
        printf("#%d", nch->ascii());
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
