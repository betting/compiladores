%{

#include <stdlib.h>
#include <stdio.h>
#include "comp_dict.h"

FILE *yyin;

%}

%union {
        
        comp_dict_item_t *symbol;
}

/* Declaração dos tokens da gramática da Linguagem K */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%left TK_OC_OR TK_OC_AND
%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc TK_PR_ELSE

%%
 /* Regras (e ações) da gramática da Linguagem K */

s: decl_global s
  | def_funcao s
  |
  ;

decl_global: decl_var ';'
  | decl_vetor ';'
  ;

decl_local: decl_var ';' decl_local
   |
   ;


 /* Declaracao de variaveis e tipos*/
decl_var: tipo_var ':' TK_IDENTIFICADOR
  ;

decl_vetor: tipo_var ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'
   ;


tipo_var: TK_PR_INT
        | TK_PR_FLOAT
        | TK_PR_BOOL
        | TK_PR_CHAR
        | TK_PR_STRING
        ;


 /* Declaracao de funcao */
def_funcao: cabecalho decl_local bloco_comando
  ;

chamada_funcao: TK_IDENTIFICADOR '(' lista_expressoes ')'

cabecalho: decl_var '(' lista_parametros ')'
  ;

lista_parametros: lista_parametros_nao_vazia
  |
  ;

lista_parametros_nao_vazia: parametro ',' lista_parametros_nao_vazia
  | parametro
  ;

parametro: decl_var
  ;

comando: bloco_comando
  | controle_fluxo
  | atribuicao ';'
  | entrada ';'
  | saida ';'
  | retorna ';'
  | decl_var ';'
  | chamada_funcao ';'
  ;

comando_simples: atribuicao
  | entrada
  | saida
  | retorna
  | decl_var
  | chamada_funcao
  ;

bloco_comando: '{' seq_comando '}'
  ;

seq_comando: comando seq_comando
  | ';'
  |
  ;


 /* Atribuicoes de variaveis */
atribuicao: TK_IDENTIFICADOR '=' expressao
  | TK_IDENTIFICADOR '[' expressao ']' '=' expressao
  ;

 /* Entrada e Saida (Input e Output) */
entrada: TK_PR_INPUT TK_IDENTIFICADOR
  ;

saida: TK_PR_OUTPUT lista_expressoes_nao_vazia 
  ;

lista_expressoes_nao_vazia: expressao ',' lista_expressoes_nao_vazia
  | expressao
  ;

retorna: TK_PR_RETURN expressao
  ;

 /* Fluxo de Controle */
controle_fluxo: TK_PR_IF '(' expressao ')' TK_PR_THEN comando %prec LOWER_THAN_ELSE
  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando
  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE comando
  | TK_PR_WHILE '(' expressao ')' TK_PR_DO comando
  ;

expressao: TK_IDENTIFICADOR
  | TK_IDENTIFICADOR '[' expressao ']'
  | expressao '+' expressao
  | expressao '-' expressao
  | expressao '*' expressao
  | expressao '/' expressao
  | expressao '<' expressao
  | expressao '>' expressao
  | '+' expressao
  | '-' expressao
  | '(' expressao ')'
  | expressao TK_OC_LE expressao
  | expressao TK_OC_GE expressao
  | expressao TK_OC_EQ expressao
  | expressao TK_OC_NE expressao
  | expressao TK_OC_AND expressao
  | expressao TK_OC_OR expressao
  | chamada_funcao
  | TK_LIT_INT
  | TK_LIT_FLOAT
  | TK_LIT_FALSE
  | TK_LIT_TRUE
  | TK_LIT_CHAR
  | TK_LIT_STRING
  ;

lista_expressoes: lista_expressoes_nao_vazia
  |
  ;

%%
