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
	int data;
	struct _tree *child,*sibling;
};
	
typedef struct _tree TREE;

/* prototipos das funções */ 
TREE* createTree();
TREE* search(TREE* root,int data)
void insertNode(int data, TREE *node);
void removeNode(TREE *node);

#endif
