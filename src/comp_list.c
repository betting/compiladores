/******************************************************
** comp_list.c
** funções de lista
******************************************************/

#include "comp_list.h"


//lista vazia
/*LIST *begin = NULL; //inicio
LIST *end = NULL;   //fim
LIST *aux;
LIST *previous; //anterior   
*/

LIST* createList()
{
	return NULL;	
}

//struct LIST *insertBegin(struct LIST *pList, int value)	
void insert(int num, LIST *node)
{
	LIST *new;
	new = malloc(sizeof(LIST));
	new->num = num;
	new->next = node->next;
	node->next = new;
}

void remove(LIST *node)
{
	LIST *dead;
	dead = node->next;
	node->next = dead->next;
	free(dead);
}



struct LIST *concatLista(struct LIST **l0, struct LIST **l1)
{
	struct LIST *L01 = createlist();
	struct LIST *L;
	if (*l0 == NULL) L01 = *l1;
	else
	{
	   L01 = *l0;
	   if (*l1 != NULL)
	   {
	       L = *l0;
	       while (L->next != NULL) L = L->next;
	       L->next = *l1;
	   }
	}
	*l0 = NULL;
	*l1 = NULL;
	return L01;
}


