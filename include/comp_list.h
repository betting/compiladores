/******************************************************
** comp_list.h
** definições e constantes para as funções de lista
******************************************************/

#ifndef _comp_list_h
#define _comp_list_h
#include <iostream>

//definicao da estrutura do elemento LISTA
struct _list{
	int num;
	_list *next;
};
	
typedef struct _list LIST;

/* prototipos das funções */ 
LIST* createList();
void insert(int num, LIST *node);
void remove(LIST *node);
struct LIST *concatLista(struct LIST **l0, struct LIST **l1);

#endif

