#ifndef _semantic_h
#define _semantic_h

#include <stdio.h>
#include "comp_tree.h"


/** CÓDIGOS DE RETORNO DE SUCESSOS/ERROS SEMÂNTICOS **/
#define IKS_SUCCESS              0 // caso não houver nenhum tipo de erro

/* Verificação de declarações */
#define IKS_ERROR_UNDECLARED     1  // identificador não declarado
#define IKS_ERROR_DECLARED       2  // identificador já declarado

/* Uso correto de identificadores */
#define IKS_ERROR_VARIABLE       3  // identificador deve ser utilizado como variável
#define IKS_ERROR_VECTOR         4  // identificador deve ser utilizado como vetor
#define IKS_ERROR_FUNCTION       5  // identificador deve ser utilizado como função

/* Tipos e tamanho de dados */
#define IKS_ERROR_WRONG_TYPE     6  // tipos incompatíveis
#define IKS_ERROR_STRING_TO_X    7  // coerção impossível do tipo string
#define IKS_ERROR_CHAR_TO_X      8  // coerção impossível do tipo char

/* Argumentos e parâmetros */
#define IKS_ERROR_MISSING_ARGS      9  // faltam argumentos
#define IKS_ERROR_EXCESS_ARGS       10 // sobram argumentos
#define IKS_ERROR_WRONG_TYPE_ARGS   11 // argumentos incompatíveis

/* Verificação de tipos em comandos */
#define IKS_ERROR_WRONG_PAR_INPUT   12 // parâmetro não é identificador
#define IKS_ERROR_WRONG_PAR_OUTPUT  13 // parâmetro não é literal string ou expressão
#define IKS_ERROR_WRONG_PAR_RETURN  14 // parâmetro não é expressão compatível com tipo do retorno

#define IKS_INT 	1
#define IKS_FLOAT 	2
#define IKS_CHAR 	3
#define IKS_STRING 	4
#define IKS_BOOL 	5

/**
 * Stack for organize 
 */ 
 
typedef struct stack
{
  comp_tree_t* disc;
  struct stack* previous;
  struct stack* next;
}STACK;

int type;
char *nameVar;
int insertLocalDeclarations(int type, comp_dict_item_t* dictNode);



STACK* initStack(void);

STACK* sPush(STACK* pointer, comp_tree_t* nodoAST);


/**
 * Check Declarations.
 * 
 * Check all variable and function declarations.
 *
 * @param *root   The tree with all elements found
 * @return The return code (sucess or error)
 */

int checkDeclarations(comp_tree_t *root);

/**
 * Check Utilization.
 * 
 * Check all variable and function utilization.
 *
 * @param *root   The tree with all elements found
 * @return The return code (sucess or error)
 */
int checkUtilization(comp_tree_t *root);

#endif
