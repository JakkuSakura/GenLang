#include "node.h"
static const char *nodeName[]
=
{
    "node","nExpression","nStatement","nInteger","nDouble","nIdentifier", "nVarType",
    "nMethodCall","nBinaryOperator","nAssignment","nBlock","nExpressionStatement",
    "nReturnStatement","nVariableDeclaration","nExternDeclaration","nFunctionDeclaration",
    "nIfStatement", "nLoopStatement", "nString", "nProgram", "nVariableDeclarationStatement",
    "nImportDeclaration", "nChar"
};
const char * getNodeName(int t)
{
    return nodeName[t];
}
