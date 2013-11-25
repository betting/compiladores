/******************************************************
** comp_list.c
** funções de lista
******************************************************/

#include <stdlib.h>
#include "comp_list.h"
#include "iks_ast.h"

/**
 * List initalization.
 *
 * Perform a list initialization.
 */
comp_list_t* initList()
{
	comp_list_t* list = malloc(sizeof(comp_list_t*));
	list = NULL;
	return list;
}
/**
 * Add new item in a list
 *
 * Add new element in the list, with the value specified.
 *
 * @param tipoVar Type variable
 * @param num value to be inserted.
 * @param *node list.
 * @param label Label of the function or size of the vector
 * @param size Size of the vector/variable
 * @return The list with the new element.
 */
comp_list_t* addItem(int tipoVar, char *nomeVar, comp_list_t* node, int tipoGlobal, int label, int size)
{
	
	comp_list_t* new;
	new = (comp_list_t *)malloc(sizeof(comp_list_t));
	new->tipoVar = tipoVar;
	new->nomeVar = nomeVar;
	new->tipoGlobal = tipoGlobal;
   new->label = label;
   new->size = size;
	new->next = NULL;

   node = add(node, new);

	return node;
}


/**
 * Adding new node to a list
 *
 * Add new element in the list, with the value specified.
 *
 * @param *list List with all elements.
 * @param *node New node to be added.
 * @return The list with the new element.
 */
comp_list_t* add(comp_list_t* list, comp_list_t* node)
{
   if (list == NULL)
   {
      list = node;
   }
   else if (list->next == NULL)
   {
      list->next = node;
   }
   else
   {
      add(list->next, node);
   }
   return list;
}

/**
 * Remove item from a list
 *
 * Remove an element from the list, with the node specified.
 *
 * @param comp_list_t node to be removed.
 */
void removeItem(comp_list_t* node)
{
	comp_list_t* dead;
	dead = node->next;
	node->next = dead->next;
	free(dead);
}

/**
 * Concat two lists
 *
 * Given two lists concat them returning one list as result
 *
 * @param comp_list_t first list to be concat.
 * @param comp_list_t second list to be concat.
 * @return comp_list_t the list resulting from the two concat
 */
comp_list_t* concatList(comp_list_t *l0, comp_list_t *l1)
{
	comp_list_t *L01;
   L01 = (comp_list_t *)malloc(sizeof(comp_list_t));

	comp_list_t *L;
   L = (comp_list_t *)malloc(sizeof(comp_list_t));

	if (l0 == NULL)
   {
      L01 = l1;
   }
	else
	{
	   L01 = l0;
	   if (l1 != NULL)
	   {
	       L = l0;
          
	       while (L->next != NULL)
          {
             L = L->next;
          }

	       L->next = l1;
          
	   }
	}
	l0 = NULL;
	l1 = NULL;
	return L01;
}

void printList(comp_list_t *list)
{
   printf("Tipo Declaracao \t Tipo de Dados \t Nome \t Label \t Size\n");

   while( list != NULL)
   {
      printf("%d \t %d \t %s \t l%d \t %d\n", list->tipoGlobal, list->tipoVar, list->nomeVar, list->label, list->size);
      list = list->next;
   }  
   
}


comp_list_t* searchToken(comp_list_t* list, char* text)
{
		comp_list_t* aux = list;

		while(aux != NULL)
		{	
			if(strcmp(aux->nomeVar, text) == 0)
         {
				return aux;
         }
			aux = aux->next;
		}

		return NULL;
}

int searchTypeToken(comp_list_t* list, char* text)
{
      comp_list_t* aux = searchToken(list, text);
      return (aux ? aux->tipoVar : 0);
}


/**
 * Return the last element from a list
 *
 * @param   *list    List with all items.
 * @return  The last element of the list.
 */
comp_list_t* getLastItemList(comp_list_t* list)
{
   comp_list_t* aux = list;

   if (list != NULL)
   {
      while(aux->next != NULL)
      {
         aux = aux->next;
      }
   }

   return aux;
}

/**
 * Get the context element from a list.
 *
 * This function returns a list starting with the first element after the item (function) given.
 * 
 * @param   *list    The list with all items.
 * @param   *function   The element to search in the list. The next element after it will be the first element of the new list.
 * @return New list with a restrict set of elements.
 */
comp_list_t* getLocalList(comp_list_t* list, comp_list_t* function)
{
   comp_list_t* aux = list;

   if ((list != NULL) && (function != NULL))
   {
      while ((strcmp(aux->nomeVar, function->nomeVar) != 0))
      {
         aux = aux->next;
      }
   }
   return aux;
}

/**
 * Get the datatype of the variable/function given
 *
 * @param   declarationType      Specify if this variable/function local or global
 * @param   *token               The token to be searched in the list
 * @param   *declarationList     List with all declarations found in the code
 * @param   *functionName        The name of the method to search local declarations. (Note: this parameter will be ignored if the declarationType is different than IKS_LOCAL)
 * @result                       The datatype of the declaration. Otherwise, it will return -1.
 */
int getDeclarationDataType(int declarationType, char* token, comp_list_t* declarationList, char* functionName)
{
   // If it's expected to search LOCAL declarations then the list will be updated
   // with all declarations available for the function given.
   if ((functionName != NULL) && ((declarationType == IKS_LOCAL) || (declarationType == IKS_FUNC_PARAM)))
   {
      declarationList = getLocalDeclarations(functionName, declarationList, declarationType);
   }

   while (declarationList != NULL)
   {
      if ((strcmp(declarationList->nomeVar, token) == 0)
            && (declarationList->tipoGlobal == declarationType))
      {
         return declarationList->tipoVar;
      }
      declarationList = declarationList->next;
   }

   return -1;
}

/**
 * Search for local declarations according to the function name given
 *
 * @param   *functionName     Name of the function
 * @param   *declarationList  List with all declarations
 * @param   declarationType   Specify if this variable/function local or global
 * @return                    All local declarations found
 */
comp_list_t* getLocalDeclarations(char *functionName, comp_list_t* declarationList, int declarationType)
{
   comp_list_t* auxList = (comp_list_t *)malloc(sizeof(comp_list_t));
   auxList = NULL;

   while (declarationList != NULL)
   {
      if ((strcmp(declarationList->nomeVar, functionName) == 0)
            && (declarationList->tipoGlobal == IKS_FUNCTION))
      {
         // If the function was found, then get the next element of the list 
         // that should be the first local declaration or the next function
         // declaration.
         declarationList = declarationList->next;
         while (declarationList != NULL)
         {
            // Getting all local declarations found for the function given
            if (declarationList->tipoGlobal == declarationType)
            {
               comp_list_t* new;
               new = (comp_list_t *)malloc(sizeof(comp_list_t));
               new->tipoVar = declarationList->tipoVar;
               new->nomeVar = declarationList->nomeVar;
               new->tipoGlobal = declarationList->tipoGlobal;
               new->next = NULL;

               auxList = add(auxList, new);
            }
            else if (!((declarationList->tipoGlobal == IKS_LOCAL) || (declarationList->tipoGlobal == IKS_FUNC_PARAM)))
            {
               // Return the local declarations for the function (this list
               // can be null if, after the actual function declaration, we
               // found another function declaration).
               return auxList;
            }

            declarationList = declarationList->next;
         }
      }
      
      // Avoiding segmentation failure if there's no more values in the list. (Last value of declarationList was gotten).
      if (declarationList != NULL)
      {
         declarationList = declarationList->next;
      }
   }

   // The function was not found in the declaration list
   return auxList;
}

int getSize(char* token, comp_list_t* declarationList)
{
   while (declarationList != NULL)
   {
      if ((strcmp(declarationList->nomeVar, token) == 0)
            && (declarationList->tipoGlobal == IKS_FUNCTION))
      {
         return declarationList->size;
      }
      declarationList = declarationList->next;
   }

   return -1;
}

