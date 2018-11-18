//
// Created by QiuJiangkun on 2018/11/9.
//

#ifndef GENLANG_PARSER_H
#define GENLANG_PARSER_H

#include "Basic.h"
#include "Scanner.h"
namespace GenLang{

    struct Node : public Object {
        explicit Node(const std::string &type);
    };

    struct Expr : public Node {
        Expr();

        explicit Expr(DynamicType *dt);

        void setVal(DynamicType *obj);

        DynamicType *getVal();

        const DynamicType *getVal() const;
    };

    struct AssignExpr : public Expr {
        AssignExpr(String *id, Expr *expr);

        std::string toString() const override;
    };

    struct ImportExpr : public Expr {
        explicit ImportExpr(String *id);

    };

    struct LetExpr : public Expr {
        List *lst;

        LetExpr();

        void append(AssignExpr *as_expr);

        std::string toString() const override;
    };

    struct FuncCall : public Expr {
        FuncCall(String *id, List *arguments);
    };

    struct Stmt : public Node {
        Stmt();

        explicit Stmt(Expr *expr);

        std::string toString() const override;
    };

    struct StmtBlock : public Stmt {
        List *list;

        explicit StmtBlock(List *lst);

        std::string toString() const override;
    };

    struct TypeKeyPair : public Node {
        TypeKeyPair(String *type, String *key);

        const std::string &getName() const;
    };

    struct FunDeclExpr : public Expr {
        TypeKeyPair *type_name_pair;
        List *arguments;
        StmtBlock *body;

        explicit FunDeclExpr(TypeKeyPair *tkp);

        void addArgument(TypeKeyPair *arg) {
            arguments->append(arg);
        }

        void setBody(StmtBlock *blk) {
            body = blk;
            append("body", blk);
        }
    };

    struct BinaryOperator : public Expr {
        BinaryOperator(String *type, Expr *left, Expr *right);
    };

    struct SingleOperator : public Expr {
        SingleOperator(String *type, Expr *right);
    };
}

#endif //PROJECT_PARSER_H
