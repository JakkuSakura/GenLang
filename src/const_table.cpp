#include <string.h>
#include <map>
#include "utils.h"
#include "node.h"
#include "genlang.parser.hh"
std::map<int, const char*> op_name;
int op_init = false;
const char *token_table_lang = "C";
void tokenStringInit()
{
    if equals(token_table_lang, "C") {
        op_name[TIMPORT] = "import";
        op_name[TEXTERN] = "extern";
        op_name[TRETURN] = "return";
        op_name[TIF] = "if";
        op_name[TFOR] = "for";
        op_name[TWHILE] = "while";
        op_name[TCONTINUE] = "continue";
        op_name[TBREAK] = "break";
        op_name[TEQUAL] = "=";
        op_name[TCEQ] = "==";
        op_name[TCNE] = "!=";
        op_name[TCLT] = "<";
        op_name[TCLE] = "<=";
        op_name[TCGT] = ">";
        op_name[TCGE] = ">=";
        op_name[TLPAREN] = "(";
        op_name[TRPAREN] = ")";
        op_name[TLBRACE] = "{";
        op_name[TRBRACE] = "}";
        op_name[TDOT] = ".";
        op_name[TCOMMA] = ",";
        op_name[TPLUS] = "+";
        op_name[TMINUS] = "-";
        op_name[TMUL] = "*";
        op_name[TDIV] = "/";
        op_name[TENDSTATEMENT] = ";";
    }else if equals(token_table_lang, "pascal") {
        op_name[TIMPORT] = "import";
        op_name[TEXTERN] = "extern";
        op_name[TRETURN] = "return";
        op_name[TIF] = "if";
        op_name[TFOR] = "for";
        op_name[TWHILE] = "while";
        op_name[TCONTINUE] = "continue";
        op_name[TBREAK] = "break";
        op_name[TEQUAL] = ":=";
        op_name[TCEQ] = "=";
        op_name[TCNE] = "!=";
        op_name[TCLT] = "<";
        op_name[TCLE] = "<=";
        op_name[TCGT] = ">";
        op_name[TCGE] = ">=";
        op_name[TLPAREN] = "(";
        op_name[TRPAREN] = ")";
        op_name[TLBRACE] = "begin";
        op_name[TRBRACE] = "end;";
        op_name[TDOT] = ".";
        op_name[TCOMMA] = ",";
        op_name[TPLUS] = "+";
        op_name[TMINUS] = "-";
        op_name[TMUL] = "*";
        op_name[TDIV] = "/";
        op_name[TENDSTATEMENT] = ";";
    }

    op_init = 1;
}
const char *getTokenString(int ch)
{
    if(!op_init) tokenStringInit();
    return op_name[ch];
}
