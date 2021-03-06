%{

#include <stdlib.h>
#include <stdio.h>
#include "comp_dict.h"
#include "comp_tree.h"
#include "comp_list.h"
#include "iks_ast.h"
#include "semantic.h"
#include "stack.h"
#include "tac.h"


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


%type<tree> 
            bloco_comando
            bloco_comando_fluxo_controle
            chamada_funcao
            comando
            comandos
            comando_simples
            controle_fluxo
            def_funcao
            entrada
            expressao
            funcao
            lista_expressoes
            lista_expressoes_nao_vazia
            nome_func
            nome_var
            nome_vetor
            vetor_dimensoes
            p
            retorna
            s
            saida
            seq_comando
            vetor

%type<symbol> cabecalho
              decl_func
              decl_var
              decl_vetor
              parametro
              TK_IDENTIFICADOR
              decl_vetor_dimensao

%type<integer> tipo_var



%right TK_OC_AND ',' '!' ';'
%left TK_OC_OR '<' '>'
%left '+' '-'
%left '*' '/'
%left '=' ')' ']'
%nonassoc TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%nonassoc LOWER_THAN_ELSE
%nonassoc TK_PR_ELSE

/*%expect 19*/

%start p
%%
 /* Regras (e ações) da gramática da Linguagem IKS */

p: s
      {
         $$ = createNode(IKS_AST_PROGRAMA,0);
         insertChild($$, $1);
         saveASTRoot($$);
//         printList(declarationList);
//         printList(listFunctions);
//         printStack(pointer);
         pointer = sPop(pointer, listFunctions, declarationList,0);
//         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
         code = initCode(code, $$, declarationList);
//         code = invertTacList(code);
//         printAssembly(code);
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
    decl_var_local
   ;

decl_var_local:
    decl_var ';' decl_local
      {
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
    tipo_var ':' TK_IDENTIFICADOR decl_vetor_dimensao
      {
         $3->type = $1;
         $$ = $3;
      }
  ;

decl_vetor_dimensao:
    '[' TK_LIT_INT ']' decl_vetor_dimensao
      { /*insere na lista de dimensões($3);*/ }
  | '[' TK_LIT_INT ']'
      { /*$$ = inicializa lista de dimensões insere na lista de dimensões($2);*/ }
  ;

tipo_var:
    TK_PR_INT { $$ = IKS_SIMBOLO_LITERAL_INT; }
  | TK_PR_FLOAT { $$ = IKS_SIMBOLO_LITERAL_FLOAT; }
  | TK_PR_BOOL { $$ = IKS_SIMBOLO_LITERAL_BOOL; }
  | TK_PR_CHAR { $$ = IKS_SIMBOLO_LITERAL_CHAR; }
  | TK_PR_STRING { $$ = IKS_SIMBOLO_LITERAL_STRING; }
  ;


 /**** DECLARACAO DE FUNCAO ****/
def_funcao:
    cabecalho decl_local bloco_comando
      {
         $$ = createNode(IKS_AST_FUNCAO, $1);
         insertChild($$, $3);
         pointer = sPush(pointer, $$);
//         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
         code = CodeGenerateFuncDeclaration($$, code, declarationList);
      }
  ;

cabecalho:
    decl_func '(' lista_parametros ')'
      {
         //printf("Func:%d:%s\n", $1->type, $1->token);
         $$ = $1;
      }
  ;

decl_func:
    tipo_var ':' TK_IDENTIFICADOR
      {
         $3->type = $1; 
         $$ = $3;         
         insertDeclarations($3, IKS_FUNCTION);

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
         insertDeclarations($1, IKS_FUNC_PARAM);
         //printf("Local(Param):%d:%s\n", $1->type, $1->token);
      }
  ;

bloco_comando:
    '{' comandos '}'
      {
         $$ = $2;
      }
  ;

comandos:
    comando_simples
      {
         $$ = $1;
      }
  | seq_comando
      {
         $$ = $1;
      }
  ;
     
seq_comando:
    controle_fluxo comandos
      {
         $$ = $1;
      }
  | nome_var '=' expressao ';' comandos
      {
//         printf("\n## CODE - ANTES ##\n");
//         printAssembly(code);
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         insertChild($$, $5);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_STORE, NULL, NULL);
//         printf("\n## CODE - DEPOIS ##\n");
//         printAssembly(code);
//         printf("\n===================\n");
      }
  | vetor '=' expressao ';' comandos
      {
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         insertChild($$, $5);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_STORE, NULL, NULL);
      }
  | nome_func '(' lista_expressoes ')' ';' comandos
      {
         $$ = createNode(IKS_AST_CHAMADA_DE_FUNCAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         insertChild($$, $6);
         pointer = sPush(pointer, $$);
//         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
         code = CodeGenerateFuncCall($$, code, declarationList);
      }
  | TK_PR_INPUT expressao ';' comandos
      {
         $$ = createNode(IKS_AST_INPUT, 0);
         insertChild($$, $2);
         insertChild($$, $4);
         pointer = sPush(pointer, $$);
      }
  | TK_PR_OUTPUT lista_expressoes_nao_vazia ';' comandos
      {
         $$ = createNode(IKS_AST_OUTPUT, 0);
         insertChild($$, $2);
         insertChild($$, $4);
         pointer = sPush(pointer, $$);
      }
  | TK_PR_RETURN expressao ';' comandos
      { 
         $$ = createNode(IKS_AST_RETURN, 0);
         insertChild($$, $2);
         insertChild($$, $4);
         pointer = sPush(pointer, $$);
         
//         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
         code = CodeGenerateReturn($$, code, declarationList);
      }
  | decl_var ';' comandos
      { }


  | bloco_comando
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
      }
  | bloco_comando ';' comandos
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
      }
  |
      { $$ = NULL; }
  ;

comando:
    controle_fluxo
      { $$ = $1; }
  | nome_var '=' expressao ';'
      {
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_STORE, NULL, NULL);
      }
  | vetor '=' expressao ';'
      {
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_STORE, NULL, NULL);
      }
  | entrada
      { $$ = $1; }
  | saida
      { $$ = $1; }
  | retorna
      { $$ = $1; }
  | declaracao
      { }
  | funcao
      { $$ = $1; }
  ;

comando_simples:
    nome_var '=' expressao
      {
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_STORE, NULL, NULL);
      }
  | vetor '=' expressao
      {
         $$ = createNode(IKS_AST_ATRIBUICAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_STORE, NULL, NULL);
      }
  ;

declaracao:
    decl_var
      { }
  | decl_var ';'
      { }
  ;

funcao:
    chamada_funcao
      {
         $$ = $1;
      }
  | chamada_funcao ';'
      {
         $$ = $1;
      }
  ;

chamada_funcao:
    nome_func '(' lista_expressoes ')'
      {
         $$ = createNode(IKS_AST_CHAMADA_DE_FUNCAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         pointer = sPush(pointer, $$);
//         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
         code = CodeGenerateFuncCall($$, code, declarationList);
      }
  ;

nome_func:
   TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
         pointer = sPush(pointer, $$);
         actualFunction = $1->token;
         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
      }
  ;


nome_var:
   TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_LOADAI, declarationList, actualFunction);
      }
  ;


 /* Entrada e Saida (Input e Output) */
entrada:
    TK_PR_INPUT expressao
      {
         $$ = createNode(IKS_AST_INPUT, 0);
         insertChild($$, $2);
         pointer = sPush(pointer, $$);
      }
  | TK_PR_INPUT expressao ';'
      {
         $$ = createNode(IKS_AST_INPUT, 0);
         insertChild($$, $2);
         pointer = sPush(pointer, $$);
      }
  ;


saida:
    TK_PR_OUTPUT lista_expressoes_nao_vazia
      {
         $$ = createNode(IKS_AST_OUTPUT, 0);
         insertChild($$, $2);
         pointer = sPush(pointer, $$);
      }
  | TK_PR_OUTPUT lista_expressoes_nao_vazia ';'
      {
         $$ = createNode(IKS_AST_OUTPUT, 0);
         insertChild($$, $2);
         pointer = sPush(pointer, $$);
      }
  ;

lista_expressoes_nao_vazia:
    expressao ',' lista_expressoes_nao_vazia
      {
         insertChild($$, $3);
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
         pointer = sPush(pointer, $$);
         
//         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
         code = CodeGenerateReturn($$, code, declarationList);
      }
  | TK_PR_RETURN expressao ';'
      { 
         $$ = createNode(IKS_AST_RETURN, 0);
         insertChild($$, $2);
         pointer = sPush(pointer, $$);
         
//         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
         code = CodeGenerateReturn($$, code, declarationList);
      }
  ;

 /* Fluxo de Controle */
controle_fluxo:
    TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle %prec LOWER_THAN_ELSE
      { 
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR, NULL, NULL);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples ';' %prec LOWER_THAN_ELSE
      { 
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR, NULL, NULL);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle TK_PR_ELSE bloco_comando_fluxo_controle
      {
//printf("IF: %s\n", $3->symbol->token);
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR, NULL, NULL);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN bloco_comando_fluxo_controle TK_PR_ELSE comando_simples ';'
      { 
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8); 
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR, NULL, NULL);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE bloco_comando_fluxo_controle
      {
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR, NULL, NULL);
      }

  | TK_PR_IF '(' expressao ')' TK_PR_THEN comando_simples TK_PR_ELSE comando_simples ';'
      {
         $$ = createNode(IKS_AST_IF_ELSE, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         insertChild($$, $8);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR, NULL, NULL);
      }

  | TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco_comando_fluxo_controle
      {
         $$ = createNode(IKS_AST_WHILE_DO, 0);
         insertChild($$, $3);
         insertChild($$, $6);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR_WHILE, NULL, NULL);
      }

  | TK_PR_DO bloco_comando_fluxo_controle TK_PR_WHILE '(' expressao ')'
      {
         $$ = createNode(IKS_AST_DO_WHILE, 0);
         insertChild($$, $2);
         insertChild($$, $5);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CBR_DO, NULL, NULL);
      }
  ;

bloco_comando_fluxo_controle:
    bloco_comando
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
         pointer = sPush(pointer, $$);

         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
      }

  | bloco_comando ';'
      {
         $$ = createNode(IKS_AST_BLOCO, 0);
         insertChild($$, $1);
         pointer = sPush(pointer, $$);

         code = CodeGenerate($$, code, ILOC_NOP, NULL, NULL);
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
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_LOADI, NULL, NULL);
      }

  | TK_LIT_FLOAT
      {
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_LOADI, NULL, NULL);
      }

  | TK_LIT_FALSE
      {
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer = sPush(pointer, $$);
      }

  | TK_LIT_TRUE
      {
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer = sPush(pointer, $$);
      }

  | TK_LIT_CHAR
      {
         $1->token = (char *)convertString($1->token);
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer = sPush(pointer, $$);
      }

  | TK_LIT_STRING
      {
         $1->token = (char *)convertString($1->token);
         $$ = createNode(IKS_AST_LITERAL, $1);
         pointer = sPush(pointer, $$);
      }
  
  | chamada_funcao
      {
         $$ = $1; 
      }

  | TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_LOADAI, declarationList, actualFunction);
      }
  | vetor
      { $$ = $1; }

  | expressao '+' expressao
      {
         $$ = createNode(IKS_AST_ARIM_SOMA, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_ADD, NULL, NULL);
      }

  | expressao '-' expressao
      {
         $$ = createNode(IKS_AST_ARIM_SUBTRACAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_SUB, NULL, NULL);
      }

  | expressao '*' expressao
      {
         $$ = createNode(IKS_AST_ARIM_MULTIPLICACAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_MULT, NULL, NULL);
      }

  | expressao '/' expressao
      {
         $$ = createNode(IKS_AST_ARIM_DIVISAO, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_DIV, NULL, NULL);
      }

  | expressao '<' expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_L, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CMP_LT, NULL, NULL);
      }

  | expressao '>' expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_G, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CMP_GE, NULL, NULL);
      }

  | '+' expressao
      { $$ = $2; }

  | '-' expressao
      {
         $$ = createNode(IKS_AST_ARIM_INVERSAO, 0);
         insertChild($$, $2);
         
         pointer = sPush(pointer, $$);
      }

  | '!' expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_NEGACAO, 0);
         insertChild($$, $2);

         pointer = sPush(pointer, $$);
      }

  | '(' expressao ')'
      { $$ = $2; }

  | expressao TK_OC_LE expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_LE, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CMP_LE, NULL, NULL);
      }

  | expressao TK_OC_GE expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_GE, 0);
         insertChild($$, $1);
         insertChild($$, $3);

         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CMP_GE, NULL, NULL);
      }

  | expressao TK_OC_EQ expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_IGUAL, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CMP_EQ, NULL, NULL);
      }

  | expressao TK_OC_NE expressao
      {
         $$ = createNode(IKS_AST_LOGICO_COMP_DIF, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_CMP_NE, NULL, NULL);
      }

  | expressao TK_OC_OR expressao
      {
         $$ = createNode(IKS_AST_LOGICO_OU, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_OR, NULL, NULL);
      }

  | expressao TK_OC_AND expressao
      {
         $$ = createNode(IKS_AST_LOGICO_E, 0);
         insertChild($$, $1);
         insertChild($$, $3);
         
         pointer = sPush(pointer, $$);
         code = CodeGenerate($$, code, ILOC_AND, NULL, NULL);
      }

  ;


vetor:
    nome_vetor vetor_dimensoes
      {
         $$ = createNode(IKS_AST_VETOR_INDEXADO, 0);
         insertChild($$, $1);
         insertChild($$, $2);
         pointer = sPush(pointer, $$);
      }
  ;

nome_vetor:
    TK_IDENTIFICADOR
      {
         $$ = createNode(IKS_AST_VETOR_INDEXADO, 0);
//         $$ = createNode(IKS_AST_IDENTIFICADOR, $1);
         pointer = sPush(pointer, $$);

         code = CodeGenerate($$, code, ILOC_LOADAI, declarationList, actualFunction);
      }
  ;
  
vetor_dimensoes:
    '[' expressao ']' vetor_dimensoes
      {
         $$ = $2;
      }
  | '[' expressao ']'
      {
         $$ = $2;
      }
  ;

lista_expressoes:
    lista_expressoes_nao_vazia
      { $$ = $1; }
  |
      { $$ = NULL; }
  ;

%%
