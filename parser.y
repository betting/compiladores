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


/*
#define IKS_AST_ARIM_SOMA           12
pra uma expressão de soma

TIPO = IKS_AST_ARIM_SOMA
gv_declare(TIPO,ponteiro do nó, lexema que não sei qual é )

o que gv_declare vai gerar
nodo_"ponteiro_do_no"[label="+"] onde: o nó (label) que vai aparecer na imagem gerada é "+" e nodo_"ponteiro_do_no" vai ser usada no gv_connect


gv_connect(nodo_"ponteiro_do_no1", nodo_"ponteiro_do_no2")


Comando pra executar o DOT e gerar a imagem do grafo em formato png
bash$ dot -Tpng nomedoarquivo -o output.png

*/


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
%token<symbol> TK_IDENTIFICADOR
%token TOKEN_ERRO

%type <tree> p s decl_global decl_local decl_var decl_vetor tipo_var def_funcao 
				chamada_funcao cabecalho lista_parametros lista_parametros_nao_vazia
				parametro comando comando_simples bloco_comando seq_comando
				atribuicao entrada saida lista_expressoes_nao_vazia retorna
				controle_fluxo expressao lista_expressoes

%left TK_OC_OR TK_OC_AND
%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc TK_PR_ELSE

%%
 /* Regras (e ações) da gramática da Linguagem K */
 
 p : s	{ $$ = $1; /*PRINT AST PASSING $$*/ }

s: decl_global s	{ $$ = createNode(IKS_AST_PROGRAMA,0); insertChild($$,$1); insertChild($$,$2); } /* gv_connect(gv_declare(IKS_AST_PROGRAMA,$$,NULL),gv_declare(IKS_AST_PROGRAMA,$$,NULL)); gv_connect(gv_declare(IKS_AST_PROGRAMA,$$,NULL),gv_declare(IKS_AST_PROGRAMA,$$,NULL)); */
  | def_funcao s	{ $$ = createNode(IKS_AST_PROGRAMA,0); insertChild($$,$1); insertChild($$,$2); }
  |					/* criar nodo para regra vazia???? */
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

chamada_funcao: TK_IDENTIFICADOR '(' lista_expressoes ')'	{ $$ = createNode(IKS_AST_CHAMADA_DE_FUNCAO,$1); insertChild($$,$3); }

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

bloco_comando: '{' seq_comando '}'	{ $$ = createNode(IKS_AST_BLOCO,0); }
  ;

seq_comando: comando seq_comando	
  | ';'								
  |	
  ;


 /* Atribuicoes de variaveis */
atribuicao: TK_IDENTIFICADOR '=' expressao				{ $$ = createNode(IKS_AST_ATRIBUICAO,$1); insertChild($$,$3); }
  | TK_IDENTIFICADOR '[' expressao ']' '=' expressao	{ $$ = createNode(IKS_AST_ATRIBUICAO,$1); insertChild($$,$3); insertChild($$,$6); }
  ;

 /* Entrada e Saida (Input e Output) */
entrada: TK_PR_INPUT TK_IDENTIFICADOR	{ $$ = createNode(IKS_AST_INPUT,$2); }
  ;

saida: TK_PR_OUTPUT lista_expressoes_nao_vazia 	{ $$ = createNode(IKS_AST_OUTPUT,0); }
  ;

lista_expressoes_nao_vazia: expressao ',' lista_expressoes_nao_vazia	
  | expressao															
  ;

retorna: TK_PR_RETURN expressao	{ $$ = createNode(IKS_AST_RETURN,0); insertChild($$,$2); }
  ;

 /* Fluxo de Controle */
controle_fluxo: TK_PR_IF '(' expressao ')' TK_PR_THEN comando %prec LOWER_THAN_ELSE	
  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando				{ $$ = createNode(IKS_AST_IF_ELSE,0); insertChild($$,$3); insertChild($$,$6); insertChild($$,$8);}
  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE comando		{ $$ = createNode(IKS_AST_IF_ELSE,0); insertChild($$,$3);insertChild($$,$6); insertChild($$,$8);}
  | TK_PR_WHILE '(' expressao ')' TK_PR_DO comando									{ $$ = createNode(IKS_AST_WHILE_DO,0); insertChild($$,$3);insertChild($$,$6);}
  | TK_PR_DO comando TK_PR_WHILE '(' expressao ')' 									{ $$ = createNode(IKS_AST_WHILE_DO,0); insertChild($$,$2);insertChild($$,$5);}
  ;

expressao: TK_IDENTIFICADOR				{ $$ = createNode(IKS_AST_IDENTIFICADOR,$1); }
  | TK_IDENTIFICADOR '[' expressao ']'	{ $$ = createNode(IKS_AST_VETOR_INDEXADO,$1); insertChild($$,$3);}
  | expressao '+' expressao				{ $$ = createNode(IKS_AST_ARIM_SOMA,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao '-' expressao				{ $$ = createNode(IKS_AST_ARIM_SUBTRACAO,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao '*' expressao				{ $$ = createNode(IKS_AST_ARIM_MULTIPLICACAO,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao '/' expressao				{ $$ = createNode(IKS_AST_ARIM_DIVISAO,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao '<' expressao				{ $$ = createNode(IKS_AST_LOGICO_COMP_L,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao '>' expressao				{ $$ = createNode(IKS_AST_LOGICO_COMP_G,0); insertChild($$,$1); insertChild($$,$3); }
  | '+' expressao						
  | '-' expressao						{ $$ = createNode(IKS_AST_ARIM_INVERSAO,0); insertChild($$,$2); }
  | '(' expressao ')'					{ $$ = $2; }
  | expressao TK_OC_LE expressao		{ $$ = createNode(IKS_AST_LOGICO_COMP_LE,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao TK_OC_GE expressao		{ $$ = createNode(IKS_AST_LOGICO_COMP_GE,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao TK_OC_EQ expressao		{ $$ = createNode(IKS_AST_LOGICO_COMP_IGUAL,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao TK_OC_NE expressao		{ $$ = createNode(IKS_AST_LOGICO_COMP_DIF,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao TK_OC_AND expressao		{ $$ = createNode(IKS_AST_LOGICO_E,0); insertChild($$,$1); insertChild($$,$3); }
  | expressao TK_OC_OR expressao		{ $$ = createNode(IKS_AST_LOGICO_OU,0); insertChild($$,$1); insertChild($$,$3); }
  | chamada_funcao						{ $$ =  $1; }
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
