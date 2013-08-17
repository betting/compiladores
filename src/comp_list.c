/******************************************************
** comp_list.c
** funções de lista
******************************************************/

#include "comp_list.h"


//lista vazia
/*comp_list_t* begin = NULL; //inicio
comp_list_t* end = NULL;   //fim
comp_list_t* aux;
comp_list_t* previous; //anterior   
*/

comp_list_t* createList()
{
	return NULL;	
}

//struct comp_list_t* insertBegin(struct comp_list_t* pList, int value)	
void addItem(int num, comp_list_t* node)
{
	comp_list_t* new;
	new = malloc(sizeof(comp_list_t*));
	new->num = num;
	new->next = node->next;
	node->next = new;
}

void removeItem(comp_list_t* node)
{
	comp_list_t* dead;
	dead = node->next;
	node->next = dead->next;
	free(dead);
}



struct comp_list_t* concatList(struct comp_list_t* *l0, struct comp_list_t* *l1)
{
	comp_list_t* L01 = createList();
	comp_list_t* L;

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


