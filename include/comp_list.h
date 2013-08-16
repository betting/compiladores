/******************************************************
** comp_list.h
** definições e constantes para as funções de lista
******************************************************/

#ifndef _comp_list_h
#define _comp_list_h
#include <stdio.h>

//definicao da estrutura do elemento LISTA
typedef struct _list
{
	int num;
	struct List *next;
} List;
	
/* prototipos das funções */ 
List* createList();
void addItem(int num, List *node);
void removeItem(List *node);
struct List *concatList(struct List **l0, struct List **l1);

#endif

