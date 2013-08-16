/******************************************************
** comp_list.c
** funções de lista
******************************************************/

#include "comp_list.h"


//lista vazia
/*List *begin = NULL; //inicio
List *end = NULL;   //fim
List *aux;
List *previous; //anterior   
*/

List* createList()
{
	return NULL;	
}

//struct List *insertBegin(struct List *pList, int value)	
void addItem(int num, List *node)
{
	List *new;
	new = malloc(sizeof(List));
	new->num = num;
	new->next = node->next;
	node->next = new;
}

void removeItem(List *node)
{
	List *dead;
	dead = node->next;
	node->next = dead->next;
	free(dead);
}



struct List *concatList(struct List **l0, struct List **l1)
{
	List *L01 = createList();
	List *L;

	if (*l0 == NULL)
   {
      L01 = *l1;
   }
	else
	{
	   L01 = *l0;
	   if (*l1 != NULL)
	   {
	       L = *l0;
          
	       while (L->next != NULL)
          {
             L = L->next;
          }

	       L->next = *l1;
          
	   }
	}
	*l0 = NULL;
	*l1 = NULL;
	return L01;
}


