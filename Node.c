//
// Created by Lenovo on 2018/9/26.
//

#include "Node.h"

static Node *nodes = NULL;

void free_nodes() {

    for (int i = 0; i < NODE_MAXN; ++i) {
        if (nodes[i].type == ID)
            free(nodes[i].STR);
        if (nodes[i].type == NODE && nodes[i].childs != NULL) {
            free(nodes[i].childs);
            nodes[i].childs = NULL;
        }

    }
    free(nodes);
    nodes = NULL;
}

pNode *new_pNode_array(int i) {
    return (pNode *) malloc(sizeof(pNode) * i);
}

pNode new_node(int flag) {
    static int cnt;
    if (nodes == NULL) {
        cnt = 0;
        nodes = (pNode) calloc(NODE_MAXN, sizeof(Node));
    }
    pNode r = &nodes[cnt++];
    r->type = flag;
    return r;

}


pNode new_node_operation(int tk) {
    Node *n = new_node(NODE);
    n->token = tk;
    return n;
}

void add_child_node(pNode r, pNode nd) {
    r->childs = realloc(r->childs, (size_t) (r->len + 1));
    r->childs[r->len + 1] = nd;
    r->len += 1;
}
