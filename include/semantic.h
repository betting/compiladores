#ifndef _semantic_h
#define _semantic_h

#include <stdio.h>
#include "comp_tree.h"
#include "comp_list.h"

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

/**
 * Stack for organize 
 */ 
 
typedef struct stack
{
  comp_tree_t* disc;
  struct stack* previous;
  struct stack* next;
}STACK;

comp_list_t* declarationList;

comp_list_t* listFunctions;

comp_list_t* listGlobal;

comp_tree_t* lastFunction;

STACK* pointer;

comp_dict_item_t* tk_var;

/**
 * Inserts declarations
 * 
 * @param *dictNode
 * @param escopo
 * @return int
 */
int insertDeclarations(comp_dict_item_t* dictNode, int escopo);

/**
 * Initialize stack
 * 
 * @return stack
 */
STACK* initStack(void);

/**
 * Inserts declarations
 * 
 * @param *pointer pointer to stack
 * @param nodoAST
 * @return stack
 */
STACK* sPush(STACK* pointer, comp_tree_t* nodoAST);

/**
 * Prints stack
 * 
 * @param stack_l
 * @return void
 */
void printStack(STACK* stack_l);

/**
 * Size declarations
 * 
 * @param type
 * @return int
 */
int sizeDeclarations(int type);

/**
 * Pop item of the stack
 * 
 * @param *pointer pointer to the stack
 * @param *function list of function declarations
 * @param *local list of local declarations
 * @param func_type
 * @return void
 */
STACK* sPop(STACK* pointer, comp_list_t* function, comp_list_t* local, int func_type);

/**
 * Add element stack?
 * 
 * @param *stack
 * @param *newElement
 * @return int
 */
STACK* addElementStack(STACK* stack, STACK* newElement);

/**
 * Validate all operations recursively
 *
 * @param   *operationNode       The first operation element found in the stack
 * @return                       The data type returned by inference function
 */
int validateOperation(comp_tree_t* operationNode);

/**
 * Calculate the difference between parameter list and parameters found in the function call.
 *
 * @param   *paramList           List with all parameters declared
 * @param   *paramFoundInCall    All parameters added in the function call
 * @return                       If result < 0 then the number of parameters used in the function call exceed the expected.
 *                               If result == 0 then both lists have the number of parameters.
 *                               Finally, if result > 0 then it was declared more parameters than the provided in the function call.
 */
int countParam(comp_list_t* paramList, comp_tree_t* paramFoundInCall);

/**
 * Validates the list of the parameters used in a function call
 *
 * @param   *paramList           List with all parameters declared
 * @param   *paramFoundInCall    All parameters added in the function call
 * @return                       Return TRUE if the parameters used are compatible
 */
int validateParam(comp_list_t* paramList, comp_tree_t* paramFoundInCall);

/**
 * Get the parameter type based on the list given
 *
 * @param   *parameter     All paramters declared
 * @result                 PArameter type
 */
int getParamType(comp_tree_t* parameter);
#endif
