#include "comp_tree.h"
#include "iks_ast.h"
#include "gv.h"

/**
 * Search a node
 *
 * Search a node given a value
 *
 * @param int	type 
 * @return comp_tree_t root
 * @return comp_tree_t 
 */
comp_tree_t* search(comp_tree_t* root, int type)
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
 * @param   type     Type of the element given (IKS_AST)
 * @param   *symbol  Dictionary item to be added in the tree
 * @return  newNode  New element (node) of the tree
 */
comp_tree_t* createNode(int type, comp_dict_item_t *symbol)
{
   // Creating new node
   comp_tree_t* newnode = (comp_tree_t*)malloc(sizeof(comp_tree_t));
   newnode->child=NULL;
   newnode->sibling=NULL;
   newnode->type=type;
   newnode->symbol=symbol;
   
   switch(type)
   {
      case IKS_AST_FUNCAO:
      case IKS_AST_LITERAL:
      case IKS_AST_IDENTIFICADOR:
 //        printf("Token: %s\n", symbol->token);
         gv_declare(type, newnode, newnode->symbol->token);
         break;

      default:
         gv_declare(type, newnode, NULL);
   }
   return newnode;
}

/**
 * Insert child in the tree
 *
 * Insert a new node in a given parent node
 *
 * @param parent  Parent node given by user where the child node will be added
 * @param child   New node to be added
 */
void insertChild(comp_tree_t* parent, comp_tree_t* child)
{
   if (child != NULL)
   {	   
      if(parent->child == NULL)
      {
		//printf("\nInserindo Child = %s",child->symbol->token);
         parent->child = child;
      }
      else
      {
 		//printf("\nINSERINDO NO SIBLING = %s",child->symbol->token);
         addSiblings(parent->child, child);
      }

      // Connect nodes in the graph
      gv_connect(parent, child);
   }
}

/**
 * Insert child (sibling) in the tree
 *
 * Add sibling node in parent node
 *
 * @param first    Reference to the node where new node will be added
 * @param newNode  New node to be added
 */
void addSiblings(comp_tree_t* first, comp_tree_t* newNode)
{
   if(first->sibling == NULL)
   {
	//printf("\nInserindo dentro do addSiblings = %s",newNode->symbol->token);
      first->sibling = newNode;
      
   }
   else
   {
      addSiblings(first->sibling, newNode);
   }
}


/**
 * Create a new node
 *
 * Create a new node given a value
 *
 * @param   type     Type of the element given (IKS_AST)
 * @param   *token   Token to be added in the dictionary
 * @return  newNode  New element (node) of the tree
 */
comp_tree_t* createNewNode(int type, char *token)
{
   // Creating new node
   comp_tree_t* newnode = (comp_tree_t*)malloc(sizeof(comp_tree_t));
   newnode->child=NULL;
   newnode->sibling=NULL;
   newnode->type=type;
   newnode->symbol = createDictionaryItem(token, "", type);
   
   return newnode;
}

