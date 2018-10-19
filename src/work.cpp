#include <iostream>
#include "node.h"
#define makeinto(r, t, n) t r = (t) n
void work(Node *node, int t)
{
    printf("%*s%s\n", t * 2, "", getNodeName(node->getNodeType()));
    switch (node->getNodeType()) {
    case nBlock:
        makeinto(n, NBlock *, node);
        const StatementList &st = n->statements;
        for (size_t i = 0; i < st.size(); i++) {
            work(st[i], t + 1);
        }
        break;
    }
}
