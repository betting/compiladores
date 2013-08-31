%{
#include <stdlib.h>
#include "comp_dict.h"
#include "parser.h"

#define IKS_SIMBOLO_INDEFINIDO         0
#define IKS_SIMBOLO_LITERAL_INT        1
#define IKS_SIMBOLO_LITERAL_FLOAT      2
#define IKS_SIMBOLO_LITERAL_CHAR       3
#define IKS_SIMBOLO_LITERAL_STRING     4
#define IKS_SIMBOLO_LITERAL_BOOL       5
#define IKS_SIMBOLO_IDENTIFICADOR      6

int lineNumber = 1;

char str[1024];

%}

%x IN_COMMENT

letter [a-zA-Z_]
digit [0-9]

%%

 /* Palavras Reservadas da Linguagem */
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


 /* Operados Compostos */
"<=" { return TK_OC_LE; }
">=" { return TK_OC_GE; }
"==" { return TK_OC_EQ; }
"!=" { return TK_OC_NE; }
"&&" { return TK_OC_AND; }
"||" { return TK_OC_OR; }


 /* Caracteres Especiais */
","|";"|":"|"("|")"|"["|"]"|"{"|"}"|"+"|"-"|"*"|"/"|"<"|">"|"="|"!"|"&"|"$" { return yytext[0]; }


 /* Literais - Identificadores */
"false" { sprintf(str, "%d", lineNumber); yylval.symbol = addDict(yytext, str, IKS_SIMBOLO_LITERAL_BOOL); return TK_LIT_FALSE; }
"true" { sprintf(str, "%d", lineNumber); yylval.symbol = addDict(yytext, str, IKS_SIMBOLO_LITERAL_BOOL); return TK_LIT_TRUE; }

{letter}({letter}|{digit})* { sprintf(str, "%d", lineNumber); yylval.symbol = addDict(yytext, str, IKS_SIMBOLO_IDENTIFICADOR); return TK_IDENTIFICADOR; }

"-"?{digit}+ { sprintf(str, "%d", lineNumber); yylval.symbol = addDict(yytext, str, IKS_SIMBOLO_LITERAL_INT); return TK_LIT_INT; }
"-"?{digit}+"."{digit}+ { sprintf(str, "%d", lineNumber); yylval.symbol = addDict(yytext, str, IKS_SIMBOLO_LITERAL_FLOAT); return TK_LIT_FLOAT; }
\'.\' { sprintf(str, "%d", lineNumber); yylval.symbol = addDict(yytext, str, IKS_SIMBOLO_LITERAL_CHAR); return TK_LIT_CHAR; }
\"[^"]*\" { sprintf(str, "%d", lineNumber); yylval.symbol = addDict(yytext, str, IKS_SIMBOLO_LITERAL_STRING); return TK_LIT_STRING; }

 /* Comentarios */
"//".* {}

<INITIAL>{
   "/*"        BEGIN(IN_COMMENT); 
}
<IN_COMMENT>{
     "*/"      BEGIN(INITIAL);
     [^*\n]+   
     "*"       
     \n        yylineno++; lineNumber++;
}


\n { lineNumber++; }
[ \t] { }
. { printf("Erro na linha %d: %s\n", lineNumber, yytext); return TOKEN_ERRO; }
%%
