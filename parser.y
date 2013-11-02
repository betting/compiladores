%{

#include <stdlib.h>
#include <stdio.h>
#include "comp_dict.h"
#include "comp_tree.h"
#include "comp_list.h"
#include "iks_ast.h"
#include "semantic.h"
#include "stack.h"


FILE *yyin;

%}

%union 
{
        comp_dict_item_t *symbol;      
        comp_tree_t *tree;
        int integer;

}

/* Declaração dos tokens da gramática da Linguagem K */

%token<symbol> TK_LIT_INT
               TK_LIT_FLOAT
               TK_LIT_FALSE
               TK_LIT_TRUE
               TK_LIT_CHAR
               TK_LIT_STRING


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
       TOKEN_ERRO
       '+'
       '-'
       '*'
       '/'
       '<'
       '>'

%token TK_IDENTIFICADOR

%token<tree> TK_PR_INT
             TK_PR_FLOAT
             TK_PR_BOOL
             TK_PR_CHAR
             TK_PR_STRING


%type<tree> atribuicao
            bloco_comando
            bloco_comando_fluxo_controle
            chamada_funcao
            comando
            comando_simples
            controle_fluxo
            def_funcao
            entrada
            expressao
            lista_expressoes
            lista_expressoes_nao_vazia
            nome_func
            nome_var
            nome_vetor
            p
            retorna
            s
            saida
            seq_comando
            valor_entrada
            vetor

%type<symbol> cabecalho
              decl_func
              decl_var
              decl_vetor
              parametro
              TK_IDENTIFICADOR

%type<integer> tipo_var



%right
   TK_OC_AND
   TK_OC_OR

%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc TK_PR_ELSE

%expect 70

%start p
%%
 /* Regras (e ações) da gramática da Linguagem IKS */

p: s
      {
         
         $$ = createNode(IKS_AST_PROGRAMA,0);
         insertChild($$, $1);
         saveASTRoot($$);
//         checkDeclarations(pointer, declarationList);
         //checkUtilization($$);
//         printList(declarationList);
         printStack(pointer);
//         printList(listFunctions);
         printf("\n\nPOINTER INVERTIDO");

         pointer = invert(pointer);
//        sPop(pointer, listFunctions, declarationList,0);
         printf("\n\nSEM FALHAS ATE AQUI");
         //pointerStack = listToStack(declarationList);
//         printStack(pointer);
         //pointerStack = listToStack(declarationList);
         //printStack2(pointerStack);
      }
  ;
s:
    decl_global s
      {
         $$ = $2;
      }

  | def_funcao s
      {
         insertChild($$, $2);
      }

  |
      { $$ = NULL; }
  ;

decl_global:
    decl_var ';'
      {
         //printf("Global:%d:%s\n", $1->type, $1->token);
         insertDeclarations($1, IKS_GLOBAL_VAR);
      }
  | decl_vetor ';'
      {
         //printf("Vetor(Global):%d:%s\n", $1->type, $1->token);
         insertDeclarations($1, IKS_GLOBAL_VET);
      }
  ;

 /* Declaracao de variaveis locais em funcoes. Nenhuma variavel pode ser declarada.*/
decl_local:
    decl_var ';' decl_local
      {
         //printf("Local:%d:%s\n", $1->type, $1->token);
         insertDeclarations($1, IKS_LOCAL);
      }
   |
   ;


 /* Declaracao de variaveis e tipos*/
decl_var:
    tipo_var ':' TK_IDENTIFICADOR
      {
         $3->type = $1;
         $$ = $3;
         /*
            INSERÇÃO NA TABELA DE SÍMBOLOS LOCAL
         */
      }
  ;

decl_vetor:
    tipo_var ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'
      {
         $3->type = $1;
         $$ = $3;
      }
  ;

tipo_var:
    TK_PR_INT { $$ = IKS_INT; }
  | TK_PR_FLOAT { $$ = IKS_FLOAT; }
  | TK_PR_BOOL { $$ = IKS_BOOL; }
  | TK_PR_CHAR { $$ = IKS_CHAR; }
  | TK_PR_STRING { $$ = IKS_STRING; }
  ;


 /**** DECLARACAO DE FUNCAO ****/
def_funcao:
    cabecalho decl_local bloco_comando
      {
         $$ = createNode(IKS_AST_FUNCAO, $1);
         insertChild($$, $3);
         pointer=sPush(pointer,$$);
      }
  ;

cabecalho:
    decl_func '(' lista_parametros ')'
      {
         insertDeclarations($1, IKS_FUNCTION);
         //printf("Func:%d:%s\n", $1->type, $1->token);
         $$ = $1;
      }
  ;

decl_func:
    tipo_var ':' TK_IDENTIFICADOR
      {
         $3->type = $1; 
         $$ = $3;         

      }
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
      {
         insertDeclarations($1, IKS_LOCAL);
         //printf("Local(Param):%d:%s\n", $1->type, $1->token);
      }
  ;

bloco_comando:
    '{' comando_simples '}'
      {
         $$ = $2;
      }
  | '{' seq_comando '}'
      {
         $$ = $2;
      }
  ;

seq_comando:
    comando seq_comando
      {
         insertChild($$, $2);
//         pointer=sPush(pointer,$$);
      }
  | comando_simples
      {
         insertChild($$, $1);
//         pointer=sPush(pointer,$$);
      }
  | bloco_comando
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
//         pointer=sPush(pointer,$$);
      }
  | bloco_comando ';' seq_comando
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
//         pointer=sPush(pointer,$$);
      }
  | seq_comando comando_simples
      {
         insertChild($$, $1);
         insertChild($$, $2);
//         pointer=sPush(pointer,$$);
      }
  | seq_comando bloco_comando
      {
         insertChild($$, $1);
         insertChild($$, $2);
//         pointer=sPush(pointer,$$);
      }
  | ';'
      { $$ = NULL; }
  |
      { $$ = NULL; }
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
      { }
  | chamada_funcao ';'
      { $$ = $1; }
  ;

comando_simples:
    atribuicao
      {
         $$ = $1;
      }

  | entrada
      { $$ = $1; }

  | saida
      { $$ = $1; }

  | retorna
      { $$ = $1; }

  | decl_var
      { }

  | chamada_funcao
      { $$ = $1; }
  ;

chamada_funcao:
    nome_func '(' lista_expressoes ')'
      {
         $$ = createNode(IKS_AST_CHAMADA_DE_FUNCAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer=sPush(pointer,$1);
//         pointer=sPush(pointer,$3);
      }
  ;

nome_func:
   TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
//         pointer=sPush(pointer,$$);
      }
  ;


 /* Atribuicoes de variaveis */
atribuicao:
    nome_var '=' expressao
      {
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_ATRIBUICAO, "=");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$1);
      }

  | vetor '=' expressao
      {
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer=sPush(pointer,$$);
      }
  ;

nome_var:
   TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
         pointer=sPush(pointer,$$);
      }
  ;


 /* Entrada e Saida (Input e Output) */
entrada:
    TK_PR_INPUT valor_entrada
      {
         $$ = createNode(IKS_AST_INPUT, 0);
         insertChild($$, $2);
//         pointer=sPush(pointer,$$);
      }
  ;

valor_entrada:
   TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
         pointer=sPush(pointer,$$);
      }
  ;

saida:
    TK_PR_OUTPUT lista_expressoes_nao_vazia
      {
         $$ = createNode(IKS_AST_OUTPUT, 0);
         insertChild($$, $2);
         pointer=sPush(pointer,$$);
      }
  ;

lista_expressoes_nao_vazia:
    expressao ',' lista_expressoes_nao_vazia
      {
         insertChild($$, $3);
         
//         pointer=sPush(pointer,$$); // Não utilizado, duplica os valores.
      }

  | expressao
      {
         $$ = $1;
      }
  ;

retorna:
    TK_PR_RETURN expressao
      { 
         $$ = createNode(IKS_AST_RETURN, 0);
         insertChild($$, $2);
//         pointer=sPush(pointer,$2);
      }
  ;

 /* Fluxo de Controle */
controle_fluxo:
    TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle %prec LOWER_THAN_ELSE
      { 
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
//         pointer=sPush(pointer,$$);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples ';' %prec LOWER_THAN_ELSE
      { 
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
//         pointer=sPush(pointer,$$);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle TK_PR_ELSE bloco_comando_fluxo_controle
      {
//printf("IF: %s\n", $3->symbol->token);
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8);
//         pointer=sPush(pointer,$$);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle TK_PR_ELSE comando_simples ';'
      { 
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8); 
//         pointer=sPush(pointer,$$);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE bloco_comando_fluxo_controle
      {
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8);
//         pointer=sPush(pointer,$$);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE comando_simples ';'
      {
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8);
//         pointer=sPush(pointer,$$);
      }

  | TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco_comando_fluxo_controle
      {
         $$ = createNode(IKS_AST_WHILE_DO, 0);
         insertChild($$, $3);
         insertChild($$, $6);
//         pointer=sPush(pointer,$$);
      }

  | TK_PR_DO bloco_comando_fluxo_controle TK_PR_WHILE '(' expressao ')'
      {
         $$ = createNode(IKS_AST_DO_WHILE, 0);
         insertChild($$, $2);
         insertChild($$, $5);
//         pointer=sPush(pointer,$$);
      }
  ;

bloco_comando_fluxo_controle:
    bloco_comando
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
//         pointer=sPush(pointer,$$);
      }

  | bloco_comando ';'
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
//         pointer=sPush(pointer,$$);
      }

  | comando
      {
         $$ = $1;
      }
  ;

expressao:
    TK_LIT_INT
      {
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer=sPush(pointer,$$);
      }

  | TK_LIT_FLOAT
      {
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer=sPush(pointer,$$);
      }

  | TK_LIT_FALSE
      {
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer=sPush(pointer,$$);
      }

  | TK_LIT_TRUE
      {
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer=sPush(pointer,$$);
      }

  | TK_LIT_CHAR
      {
         $1->token = (char *)convertString($1->token);
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer=sPush(pointer,$$);
      }

  | TK_LIT_STRING
      {
         $1->token = (char *)convertString($1->token);
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer=sPush(pointer,$$);
      }
  
  | chamada_funcao
      {
         $$ = $1; 
//         pointer=sPush(pointer,$$);
      }

  | TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
         pointer=sPush(pointer,$$);
      }
  | vetor
      { $$ = $1; }

  | expressao '+' expressao
      {
         $$ = createNode(IKS_AST_ARIM_SOMA, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_ARIM_SOMA, "+");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao '-' expressao
      {
         $$ = createNode(IKS_AST_ARIM_SUBTRACAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_ARIM_SUBTRACAO, "-");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao '*' expressao
      {
         $$ = createNode(IKS_AST_ARIM_MULTIPLICACAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_ARIM_MULTIPLICACAO, "*");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao '/' expressao
      {
         $$ = createNode(IKS_AST_ARIM_DIVISAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_ARIM_DIVISAO, "/");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao '<' expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_L, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_COMP_L, "<");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao '>' expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_G, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_COMP_G, ">");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | '+' expressao
      { $$ = $2; }

  | '-' expressao
      {
         $$ = createNode(IKS_AST_ARIM_INVERSAO, 0);
         insertChild($$, $2);
         
         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_ARIM_INVERSAO, "-");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | '!' expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_NEGACAO, 0);
         insertChild($$, $2);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_COMP_NEGACAO, "!");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | '(' expressao ')'
      { $$ = $2; }

  | expressao TK_OC_LE expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_LE, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_COMP_LE, "<=");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao TK_OC_GE expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_GE, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_COMP_GE, ">=");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao TK_OC_EQ expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_IGUAL, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_COMP_IGUAL, "==");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao TK_OC_NE expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_DIF, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_COMP_DIF, "!=");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao TK_OC_OR expressao
      {
         $$ = createNode(IKS_AST_LOGICO_OU, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_OU, "||");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  | expressao TK_OC_AND expressao
      {
         $$ = createNode(IKS_AST_LOGICO_E, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         comp_tree_t *treeItem;
         treeItem = createNewNode(IKS_AST_LOGICO_E, "&&");

         pointer=sPush(pointer, treeItem);
//         pointer=sPush(pointer,$$);
      }

  ;


vetor:
    nome_vetor '[' expressao ']'
      {
         $$ = createNode(IKS_AST_VETOR_INDEXADO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
//         pointer=sPush(pointer,$$);
      }
  ;

nome_vetor:
    TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
//         pointer=sPush(pointer,$$);
      }
  ;

lista_expressoes:
    lista_expressoes_nao_vazia
      { $$ = $1; }
  |
      { $$ = NULL; }
  ;

%%
