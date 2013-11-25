/******************************************************
** comp_list.h
** definições e constantes para as funções de lista
******************************************************/
#ifndef _comp_list_h
#define _comp_list_h

#include <stdio.h>

typedef struct _list
{
	int tipoVar;
	char *nomeVar;
   int tipoGlobal;
   int label;
   int size;
	struct _list *next;
} comp_list_t;

/**
 * List initalization.
 *
 * Perform a list initialization following the size set in a constant.
 * 
 * @return void
 */
comp_list_t* initList();

/**
 * Add new item in a list.
 *
 * @param tipoVar type of variable
 * @param *nomeVar name of variable
 * @param *node the list
 * @param tipoGlobal global type
 * @param label Label of the function or size of the vector
 * @param size Size of the vector/variable
 * @return Dictionary item created (Key/Value).
 */
comp_list_t* addItem(int tipoVar, char *nomeVar, comp_list_t* node, int tipoGlobal, int label, int size);

/**
 * Add new item in a list.
 *
 * @param *list the list
 * @param *node node to be added
 * @return comp_list_t the list
 */
comp_list_t* add(comp_list_t* list, comp_list_t* node);

/**
 * Add new item in a list.
 *
 * @param *node to remove
 * @return void
 */
void removeItem(comp_list_t* node);

/**
 * Concatenate two lists
 *
 * @param *l0 list one
 * @param *l1 list two
 * @return comp_list_t new list
 */
comp_list_t* concatList(comp_list_t *l0, comp_list_t *l1);

/**
 * Prints a list
 *
 * @param *list
 * @return void
 */
void printList(comp_list_t *list);

/**
 * Concatenate two lists
 *
 * @param *list the list
 * @param *text token to search
 * @return comp_list_t list node containing token
 */
comp_list_t* searchToken(comp_list_t* list, char* text);

/**
 * Returns last item of the list
 *
 * @param *list the list
 * @return comp_list_t last item of the list
 */
comp_list_t* getLastItemList(comp_list_t* list);

/**
 * Returns local list
 *
 * @param *list
 * @param *function
 * @return comp_list_t local list
 */
comp_list_t* getLocalList(comp_list_t* list, comp_list_t* function);

/**
 * Get the datatype of the variable/function given
 *
 * @param   declarationType      Specify if this variable/function local or global
 * @param   *token               The token to be searched in the list
 * @param   *declarationList     List with all declarations found in the code
 * @param   *functionName        The name of the method to search local declarations. (Note: this parameter will be ignored if the declarationType is different than IKS_LOCAL)
 * @result                       The datatype of the declaration. Otherwise, it will return -1.
 */
int getDeclarationDataType(int declarationType, char* token, comp_list_t* declarationList, char* functionName);

/**
 * Search for local declarations according to the function name given
 *
 * @param   *functionName     Name of the function
 * @param   *declarationList  List with all declarations
 * @param   declarationType   Specify if this variable/function local or global
 * @return                    All local declarations found
 */
comp_list_t* getLocalDeclarations(char *functionName, comp_list_t* declarationList, int declarationType);

int getSize(char* token, comp_list_t* declarationList);
#endif
