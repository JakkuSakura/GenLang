//
// Created by Lenovo on 2018/9/26.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include <stdlib.h>
#include <memory.h>

typedef struct __node
{
    int type;
    union {
        union {
            double F64;
            long long I64;
            char *STR;
        }val;
        struct {
            int token;
            int len;
            struct __node **childs;
        }expr;
    }un;
}Node;
typedef Node *pNode;
#define YYSTYPE pNode

#include "y.tab.h"

#define NODE_MAXN 50000


Node **new_node_array(int i);
Node *new_node();
Node *new_node_operation(int tk);
void free_nodes();
#endif //PROJECT_NODE_H
