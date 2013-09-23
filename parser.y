%{

#include <stdlib.h>
#include <stdio.h>
#include "comp_dict.h"
#include "comp_tree.h"
#include "iks_ast.h"

FILE *yyin;

%}

%union {
        
        comp_dict_item_t *symbol;
        comp_tree_t *tree;
}

/* Declaração dos tokens da gramática da Linguagem K */
%token<symbol> TK_PR_INT
               TK_PR_FLOAT
               TK_PR_BOOL
               TK_PR_CHAR
               TK_PR_STRING

%token TK_PR_IF
       TK_PR_THEN
       TK_PR_ELSE
       TK_PR_WHILE
       TK_PR_DO
       TK_PR_INPUT
       TK_PR_OUTPUT
       TK_PR_RETURN
       TK_OC_LE
       TK_OC_GE
       TK_OC_EQ
       TK_OC_NE
       TK_OC_AND
       TK_OC_OR
       TK_LIT_INT
       TK_LIT_FLOAT
       TK_LIT_FALSE
       TK_LIT_TRUE
       TK_LIT_CHAR
       TK_LIT_STRING
       TOKEN_ERRO
       TK_IDENTIFICADOR
       '+'
       '-'
       '*'
       '/'
       '<'
       '>'


%type<tree> atribuicao
            bloco_comando
            cabecalho
            chamada_funcao
            comando
            comando_simples
            controle_fluxo
            decl_var
            def_funcao
            entrada
            expressao
            lista_expressoes
            lista_expressoes_nao_vazia
            parametro
            retorna
            saida
            tipo_var

%type<symbol> seq_comando

%left TK_OC_OR TK_OC_AND
%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc TK_PR_ELSE

%start s
%%
 /* Regras (e ações) da gramática da Linguagem IKS */
 
s:
    decl_global s
      { /*$$ = createNode(IKS_AST_PROGRAMA,0); insertChild($$, $2);*/ }

  | def_funcao s
      { /*$$ = createNode(IKS_AST_PROGRAMA,0); insertChild($$, $2);*/ }

  |					
  ;

decl_global:
    decl_var ';'
  | decl_vetor ';'			
  ;

 /* Declaracao de variaveis locais em funcoes. Nenhuma variavel pode ser declarada.*/
decl_local:
    decl_var ';' decl_local
   |
   ;


 /* Declaracao de variaveis e tipos*/
decl_var:
    tipo_var ':' TK_IDENTIFICADOR
  ;

decl_vetor:
    tipo_var ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'
  ;


tipo_var:
    TK_PR_INT
  | TK_PR_FLOAT
  | TK_PR_BOOL
  | TK_PR_CHAR
  | TK_PR_STRING
  ;


 /**** DECLARACAO DE FUNCAO ****/
def_funcao:
    cabecalho decl_local bloco_comando
      { /* $$ = createNode(IKS_AST_FUNCAO, 0); insertChild($$, $3); */ }
  ;

cabecalho:
    decl_var '(' lista_parametros ')'
  ;

lista_parametros:
    lista_parametros_nao_vazia
  |	
  ;

lista_parametros_nao_vazia:
    parametro ',' lista_parametros_nao_vazia
  | parametro															
  ;

parametro:
    decl_var
  ;

bloco_comando:
    '{' seq_comando '}'
      { $$ = createNode(IKS_AST_BLOCO,$2); }
  | '{' comando_simples '}'
  ;

seq_comando:
    comando seq_comando
  | comando ';'
  | comando_simples
  | bloco_comando
  | bloco_comando ';' seq_comando
  | seq_comando comando_simples
  | seq_comando bloco_comando
  | ';'
  |
  ;

comando:
    controle_fluxo
      { $$ = $1; }
  | atribuicao ';'
      { $$ = $1; }
  | entrada ';'
      { $$ = $1; }
  | saida ';'
      { $$ = $1; }
  | retorna ';'
      { $$ = $1; }
  | decl_var ';'
      { $$ = $1; }
  | chamada_funcao ';'
      { $$ = $1; }
  ;

comando_simples:
    atribuicao
      { $$ = $1; }

  | entrada
      { $$ = $1; }

  | saida
      { $$ = $1; }

  | retorna
      { $$ = $1; }

  | decl_var
      { $$ = $1; }

  | chamada_funcao
      { $$ = $1; }
  ;

chamada_funcao:
    TK_IDENTIFICADOR '(' lista_expressoes ')'
      { /* $$ = createNode(IKS_AST_CHAMADA_DE_FUNCAO, $1); */ }
  ;

 /* Atribuicoes de variaveis */
atribuicao:
    TK_IDENTIFICADOR '=' expressao
      { /* $$ = createNode(IKS_AST_ATRIBUICAO, $1); insertChild($$, $3); */ }

  | TK_IDENTIFICADOR '[' expressao ']' '=' expressao
      { /* $$ = createNode(IKS_AST_ATRIBUICAO, $1); insertChild($$, $3); insertChild($$, $6); */ }
  ;

 /* Entrada e Saida (Input e Output) */
entrada:
    TK_PR_INPUT TK_IDENTIFICADOR
      { /* $$ = createNode(IKS_AST_INPUT, $2); */ }
  ;

saida:
    TK_PR_OUTPUT lista_expressoes_nao_vazia
      { $$ = createNode(IKS_AST_OUTPUT, $2); }
  ;

lista_expressoes_nao_vazia:
    expressao ',' lista_expressoes_nao_vazia
      { $$ = $1; }

  | expressao
      { $$ = $1; }
  ;

retorna:
    TK_PR_RETURN expressao
      { /* $$ = createNode(IKS_AST_RETURN,$2); insertChild($$,$2); */ }
  ;

 /* Fluxo de Controle */
controle_fluxo:
    TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle %prec LOWER_THAN_ELSE
      { /* $$ = createNode(IKS_AST_IF_ELSE, 0); insertChild($$, $3); insertChild($$, $6); */ }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle TK_PR_ELSE bloco_comando_fluxo_controle
      { /* $$ = createNode(IKS_AST_IF_ELSE, 0); insertChild($$, $3); insertChild($$, $6); insertChild($$, $8); */ }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle TK_PR_ELSE comando_simples ';'
      { /* $$ = createNode(IKS_AST_IF_ELSE, 0); insertChild($$, $3); insertChild($$, $6); insertChild($$, $8); */ }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE bloco_comando_fluxo_controle
      { /* $$ = createNode(IKS_AST_IF_ELSE, 0); insertChild($$, $3);insertChild($$, $6); insertChild($$, $8); */ }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE comando_simples ';'
      { /* $$ = createNode(IKS_AST_IF_ELSE, 0); insertChild($$, $3);insertChild($$, $6); insertChild($$, $8); */ }

  | TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco_comando_fluxo_controle
      { /* $$ = createNode(IKS_AST_WHILE_DO, 0); insertChild($$, $3);insertChild($$, $6); */ }

  | TK_PR_DO bloco_comando_fluxo_controle TK_PR_WHILE '(' expressao ')'
      { /* $$ = createNode(IKS_AST_DO_WHILE, 0); insertChild($$, $2);insertChild($$, $5); */ }
  ;

bloco_comando_fluxo_controle:
    bloco_comando
  | bloco_comando ';'
  | comando
  ;

expressao:
    TK_IDENTIFICADOR
      { /* $$ = createNode(IKS_AST_IDENTIFICADOR, $1);*/ }

  | TK_IDENTIFICADOR '[' expressao ']'
      { /* $$ = createNode(IKS_AST_VETOR_INDEXADO, $1); insertChild($$, $3); */ }

  | expressao '+' expressao
      { /* $$ = createNode(IKS_AST_ARIM_SOMA, $2); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao '-' expressao
      { /* $$ = createNode(IKS_AST_ARIM_SUBTRACAO, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao '*' expressao
      { /* $$ = createNode(IKS_AST_ARIM_MULTIPLICACAO, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao '/' expressao
      { /* $$ = createNode(IKS_AST_ARIM_DIVISAO, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao '<' expressao
      { /* $$ = createNode(IKS_AST_LOGICO_COMP_L, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao '>' expressao
      { /* $$ = createNode(IKS_AST_LOGICO_COMP_G, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | '+' expressao
      { $$ = $2; }

  | '-' expressao
      { $$ = $2; /* $$ = createNode(IKS_AST_ARIM_INVERSAO, 0); insertChild($$, $2); */ }

  | '!' expressao
      { $$ = $2; }

  | '(' expressao ')'
      { $$ = $2; }

  | expressao TK_OC_LE expressao
      { /* $$ = createNode(IKS_AST_LOGICO_COMP_LE, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao TK_OC_GE expressao
      { /* $$ = createNode(IKS_AST_LOGICO_COMP_GE, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao TK_OC_EQ expressao
      { /* $$ = createNode(IKS_AST_LOGICO_COMP_IGUAL, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao TK_OC_NE expressao
      { /* $$ = createNode(IKS_AST_LOGICO_COMP_DIF, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao TK_OC_AND expressao
      { /* $$ = createNode(IKS_AST_LOGICO_E, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | expressao TK_OC_OR expressao
      { /* $$ = createNode(IKS_AST_LOGICO_OU, 0); insertChild($$, $1); insertChild($$, $3); */ }

  | chamada_funcao
      { /* $$ = createNode(IKS_AST_CHAMADA_DE_FUNCAO, 0); */ }

  | TK_LIT_INT
      { /*$$ = createNode(IKS_AST_LITERAL, $1);*/ }

  | TK_LIT_FLOAT
      { /*$$ = createNode(IKS_AST_LITERAL, $1);*/ }

  | TK_LIT_FALSE
      { /*$$ = createNode(IKS_AST_LITERAL, $1);*/ }

  | TK_LIT_TRUE
      { /*$$ = createNode(IKS_AST_LITERAL, $1);*/ }

  | TK_LIT_CHAR
      { /*$$ = createNode(IKS_AST_LITERAL, $1);*/ }

  | TK_LIT_STRING
      { /*$$ = createNode(IKS_AST_LITERAL, $1);*/ }
  ;

lista_expressoes:
    lista_expressoes_nao_vazia
      { $$ = $1; }
  |
  ;

%%
