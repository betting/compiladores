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

typedef struct _tree
{
	int type;
	int node_type;
	int size;
	TAC* code;
	comp_dict_item_t* symbol;
	struct _tree *child,*sibling;
} comp_tree_t;

/**
 * Search data in tree
 *
 * @param *root tree root
 * @param data
 * @return comp_tree_t tree
 */
comp_tree_t* search(comp_tree_t* root,int data);

/**
 * Concatenate two lists
 *
 * @param type
 * @param *symbol
 * @return comp_tree_t new node
 */
comp_tree_t* createNode(int type, comp_dict_item_t *symbol);

/**
 * Insert child in a node
 *
 * @param *parent
 * @param *child
 * @return void
 */
void insertChild(comp_tree_t* parent, comp_tree_t* child);

/**
 * Add siblings of a node
 * 
 * Each node has its list of sons, add siblings add new nodes in this list of sons of a given node
 *
 * @param *parent
 * @param *child
 * @return void
 */
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
