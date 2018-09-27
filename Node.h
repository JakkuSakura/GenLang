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
        };
        struct {
            int token;
            int len;
            struct __node **childs;
        };
    };
}Node;
typedef Node *pNode;
#define YYSTYPE pNode

#include "y.tab.h"

#define NODE_MAXN 50000


pNode *new_pNode_array(int i);
pNode new_node(int flag);
void add_child_node(pNode r, pNode nd);
pNode new_node_operation(int tk);
void free_nodes();
#endif //PROJECT_NODE_H
