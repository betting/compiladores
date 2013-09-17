/******************************************************
** comp_tree.h
** definições e constantes para as funções de arvore
******************************************************/
#ifndef _comp_tree_h
#define _comp_tree_h

#include <stdio.h>
#include <stdlib.h>

//definicao da estrutura do elemento LISTA
typedef struct _tree
{
	int type;
	struct comp_dict_item_t* symbol;
	struct _tree *child,*sibling;
} comp_tree_t;

/* prototipos das funções */ 
comp_tree_t* search(comp_tree_t* root,int data);
comp_tree_t* createNode(int type, struct comp_dict_item_t* symbol);
void insertChild(comp_tree_t* parent, comp_tree_t* child);
void addSiblings(comp_tree_t* first, comp_tree_t* newNode);
void preorder(comp_tree_t* p);
#endif
