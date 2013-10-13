/******************************************************
** comp_list.c
** funções de lista
******************************************************/

#include <stdlib.h>
#include "comp_list.h"

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
 * @return The list with the new element.
 */
comp_list_t* addItem(int tipoVar, char *nomeVar, comp_list_t* node, int tipoGlobal)
{
	
	comp_list_t* new;
	new = (comp_list_t *)malloc(sizeof(comp_list_t));
	new->tipoVar = tipoVar;
	new->nomeVar = nomeVar;
	new->tipoGlobal = tipoGlobal;
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
   printf("\nLISTAR\n");
    
    int i=0;
    while( list != NULL)    /* Enquanto nao chega no fim da lista */
    {
        i++;
		printf("DENTRO DO WHILE\n");
        printf("\n\nTipoVar %d - NomeVar: %s \n", list->tipoVar, list->nomeVar);
        list = list->next;     /* Faz noatual apontar para o proximo no */
    }
}

comp_list_t* searchToken(comp_list_t* list, char* text)
{
		comp_list_t* aux = list;

		while(aux != NULL)
		{	
			if(strcmp(aux->nomeVar, text) == 0)
				return aux;
			aux = aux->next;
		}

		return NULL;
}

