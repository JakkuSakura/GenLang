//
// Created by Lenovo on 2018/9/26.
//

#include "Node.h"
static Node *nodes = NULL;
void free_nodes() {

    for (int i = 0; i < NODE_MAXN; ++i) {
        if (nodes[i].type == ID)
            free(nodes[i].un.val.STR);
        if (nodes[i].type == NODE && nodes[i].un.expr.childs != NULL)
        {
            free(nodes[i].un.expr.childs);
            nodes[i].un.expr.childs = NULL;
        }

    }
    free(nodes);
    nodes = NULL;
}

Node **new_node_array(int i) {
    return (Node **)malloc(sizeof(Node *) * i);
}
Node *new_node() {
    static int cnt;
    if (nodes == NULL)
    {
        cnt = 0;
        nodes = (Node *)malloc(sizeof(Node) * NODE_MAXN);
        memset(nodes, 0, sizeof(Node) * NODE_MAXN);
    }
    return &nodes[cnt ++];

}


Node *new_node_operation(int tk) {
    Node *n = new_node();
    n->type = NODE;
    n->un.expr.token = tk;
    return n;
}
