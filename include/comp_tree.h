/******************************************************
** comp_tree.h
** definições e constantes para as funções de arvore
******************************************************/
#ifndef _comp_tree_h
#define _comp_tree_h

#include <stdio.h>
#include <stdlib.h>

//definicao da estrutura do elemento LISTA
struct _tree
{
	int data;
	struct _tree *child,*sibling;
};
	
typedef struct _tree comp_tree_t;

/* prototipos das funções */ 
comp_tree_t* search(comp_tree_t* root,int data);
comp_tree_t* initTree(comp_tree_t* root,int data[]);
void createNode(int data);

#endif
