/******************************************************
** comp_list.c
** funções de lista
******************************************************/

#include <stdlib.h>
#include "comp_list.h"


//lista vazia
/*comp_list_t* begin = NULL; //inicio
comp_list_t* end = NULL;   //fim
comp_list_t* aux;
comp_list_t* previous; //anterior   
*/

/**
 * List initalization.
 *
 * Perform a list initialization.
 */
comp_list_t* initList()
{
	return NULL;	
}
/**
 * Add new item in a list
 *
 * Add new element in the list, with the value specified.
 *
 * @param num value to be inserted.
 * @param *node list.
 */
void addItem(int num, comp_list_t* node)
{
	comp_list_t* new;
	new = (comp_list_t *)malloc(sizeof(comp_list_t));
	new->num = num;
	new->next = node->next;
	node->next = new;
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


