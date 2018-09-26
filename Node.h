//
// Created by Lenovo on 2018/9/26.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include <stdlib.h>
#include <memory.h>
#include "y.tab.h"

typedef struct __node
{
    int type;
    union {
        YYSTYPE val;
        struct {
            int len;
            struct __node *childs;
        };
    }un;
}Node;
#define NODE_MAXN 50000
Node *nodes = nullptr;
Node *new_node()
{
    static int cnt;
    if (nodes == nullptr)
    {
        cnt = 0;
        nodes = (Node *)malloc(sizeof(Node) * NODE_MAXN);
        memset(nodes, 0, sizeof(Node) * NODE_MAXN);
    }
    return &nodes[cnt ++];

}
void free_nodes()
{
    for (int i = 0; i < NODE_MAXN; ++i) {
        if (nodes[i].type == ID)
            free(nodes[i].un.val.STR);
    }
    free(nodes);
    nodes = nullptr;
}
#endif //PROJECT_NODE_H
