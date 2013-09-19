#include "comp_tree.h"
#include "gv.h"

void preorder(comp_tree_t* p)
{
    if(p==NULL)
    {
       return;
    }
    printf(" %d",p->type);
    preorder(p->child);

    preorder(p->sibling);
}

/**
 * Search a node
 *
 * Search a node given a value
 *
 * @param int	type 
 * @return comp_tree_t root
 * @return comp_tree_t 
 */
comp_tree_t* search(comp_tree_t* root,int type)
{
    if(root == NULL)
    {
      return;
    }

    if(type == root->type)
    {
      return root;
    }

    comp_tree_t* t = search(root->child,type);

    if(t == NULL)
    {
      t = search(root->sibling,type);
    }
    return t;

}

/**
 * Create a node
 *
 * Create a node given a value
 *
 * @param int	type 
 * @return comp_tree_t newNode
 */
comp_tree_t* createNode(int type, comp_dict_item_t *symbol)
{
    comp_tree_t* newnode= (comp_tree_t*)malloc(sizeof(comp_tree_t));
    newnode->child=NULL;
    newnode->sibling=NULL;
    newnode->type=type;
    newnode->symbol=symbol;
    
    //cria nodo no grafo
    gv_declare(type,newnode,NULL);
    
    return newnode;
}

/**
 * Insert a node in a given parent node 
 * 
 **/
 void insertChild(comp_tree_t* parent, comp_tree_t* child)
 {
	 if(parent->child == NULL)
    {
		 parent->child = child;
	 }
    else 
    {
		 addSiblings(parent->child, child);
	 }
	 
	 //conecta nodos no grafo
	 gv_connect(parent, child);
 }
 

 void addSiblings(comp_tree_t* first, comp_tree_t* newNode)
 {
	 if(first->sibling == NULL)
    {
		 first->sibling = newNode;
	 }
    else
    {
		 addSiblings(first->sibling, newNode);
	 }
 }
