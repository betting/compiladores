/******************************************************
** comp_tree.h
** definições e constantes para as funções de arvore
******************************************************/
#ifndef _comp_tree_h
#define _comp_tree_h

#define MAX_CHILDREN 2
#include <iostream>
#include "comp_list.h"

//definicao da estrutura do elemento LISTA
struct _tree{
	int num;
	LIST *children;
};
	
typedef struct _tree TREE;

/* prototipos das funções */ 
TREE* createTree();
void insertNode(int num, TREE *node);
void removeNode(TREE *node);
struct TREE *concatLista(struct TREE **l0, struct TREE **l1);

#endif
