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
	struct _list *next;
} comp_list_t;
	
/* prototipos das funções */ 
comp_list_t* initList();
void addItem(int num, comp_list_t* node);
void removeItem(comp_list_t* node);
comp_list_t* concatList(comp_list_t *l0, comp_list_t *l1);

#endif
