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
	
typedef struct _tree Tree;

/* prototipos das funções */ 
Tree* createTree();
Tree* createTree(Tree* root,int data[]);

Tree* search(Tree* root,int data);
void insertNode(int data, Tree *node);
void removeNode(Tree *node);

#endif
