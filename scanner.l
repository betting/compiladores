%{
#include "tokens.h"
%}

letter [a-zA-Z_]
digit [0-9]

%%

int { return TK_PR_INT; }
float { return TK_PR_FLOAT; }
bool { return TK_PR_BOOL; }
char { return TK_PR_CHAR; }
string { return TK_PR_STRING; }
if { return TK_PR_IF; }
then { return TK_PR_THEN; }
else { return TK_PR_ELSE; }
while { return TK_PR_WHILE; }
do { return TK_PR_DO; }
input { return TK_PR_INPUT; }
output { return TK_PR_OUTPUT; }
return { return TK_PR_RETURN; }


","|";"|":"|"("|")"|"["|"]"|"{"|"}"|"+"|"-"|"*"|"/"|"<"|">"|"="|"!"|"&"|"$" { return yytext[0]; }

"<=" { return TK_OC_LE; }
">=" { return TK_OC_GE; }
"==" { return TK_OC_EQ; }
"!=" { return TK_OC_NE; }
"&&" { return TK_OC_AND; }
"||" { return TK_OC_OR; }

"false" { return TK_LIT_FALSE; }
"true" { return TK_LIT_TRUE; }

{letter}({letter}|{digit})* { return TK_IDENTIFICADOR; }

"-"?{digit}+ { return TK_LIT_INT; }
"-"?{digit}+"."{digit}+ { return TK_LIT_FLOAT; }
\'.\' { return TK_LIT_CHAR; }
\"*\" { return TK_LIT_STRING; }

"//".* {}

. { return TOKEN_ERRO; }
%%