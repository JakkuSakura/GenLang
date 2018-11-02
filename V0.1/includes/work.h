#ifndef __GENLANG_WORK_H
#define __GENLANG_WORK_H
#include "node.h"
void print(const Node *node, int t);
void walk(const Node *node, void (*f)(const Node *));
void pas_code(const NProgram *node);
extern const NFunctionDeclaration *now_function;
#endif
