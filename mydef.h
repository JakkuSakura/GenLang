//
// Created by Lenovo on 2018/9/22.
//

#ifndef PROJECT_MYDEF_H
#define PROJECT_MYDEF_H
#include "y.tab.h"
typedef struct tagNode{
    int type;
    union
    {
        double F64;
        int64_t I64;
        char *STR;
        struct{
            int type;
            int len;
            struct tagNode *son;
        }OPER;
    }val;
} TreeNode;
#endif //PROJECT_MYDEF_H
