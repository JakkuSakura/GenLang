#include "node.h"
static const char *nodeName[]
=
{
    "node","nExpression","nStatement","nInteger","nDouble","nIdentifier", "nVarType",
    "nMethodCall","nBinaryOperator","nAssignment","nBlock","nExpressionStatement",
    "nReturnStatement","nVariableDeclaration","nExternDeclaration","nFunctionDeclaration",
    "nIfStatement", "nLoopStatement"
};
const char * getNodeName(nodetype t)
{
    return nodeName[t];
}
