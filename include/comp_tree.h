/******************************************************
** comp_tree.h
** definições e constantes para as funções de arvore
******************************************************/
#ifndef _comp_tree_h
#define _comp_tree_h

#include <stdio.h>
#include <stdlib.h>
#include "comp_dict.h"

struct comp_dict_item_t;

//definicao da estrutura do elemento LISTA
typedef struct _tree
{
	int type;
	int size;
	comp_dict_item_t* symbol;
	struct _tree *child,*sibling;
} comp_tree_t;

/* prototipos das funções */ 
comp_tree_t* search(comp_tree_t* root,int data);
comp_tree_t* createNode(int type, comp_dict_item_t *symbol);
void insertChild(comp_tree_t* parent, comp_tree_t* child);
void addSiblings(comp_tree_t* first, comp_tree_t* newNode);

/**
 * Create a new node
 *
 * Create a new node given a value
 *
 * @param   type     Type of the element given (IKS_AST)
 * @param   *token   Token to be added in the dictionary
 * @return  newNode  New element (node) of the tree
 */
comp_tree_t* createNewNode(int type, char *token);

#endif
