#include <stdlib.h>
#include "comp_graph.h"


//---------------------------------------------------
//                Create Graph
//---------------------------------------------------
/*
void createGraph(struct node *h, int item)
{
	h->item = item;
	h->visit = 0;
	//aloca o espaço para o próximo
	h->next = (struct node *)malloc(sizeof(struct node));
}*/

/**
 * Create a Graph
 *
 * Initialization of a graph
 * Allocation of memory
 *
 * @param int	item
 */
void createGraph(comp_graph_t *h, int item)
{
	h->item = item;
	h->visit = 0;
	//aloca o espaço para o próximo
	h->next = (comp_graph_t *)malloc(sizeof(comp_graph_t));
}

/**
 * Search a node in Graph
 *
 * Search a node given a value
 *
 * @param int p 
 * @return comp_tree_t 
 */
comp_graph_t *searchGraph(comp_graph_t *h,int p)
{
 while(h!=NULL)
 {
  if(h->item==p)
  {return h;}
  else
  { h=h->next;}
 }
 return NULL;
}

/**
 * Connect a node and a link in certain Graph
 *
 * Search a node given a value
 *
 * @param int value 
 * @return comp_graph_t
 *  
 */
void connectNodeGraph(comp_graph_t *h,  link *l, int value)
{
  if(h==NULL)return;
  if(l==NULL)
  {
	  l=(link*)malloc(sizeof(link));
	  h->adjacent=l;
	  l->points=searchGraph(h,value);
	  l->next=(link*)malloc(sizeof(link));
   }	
}
/**
 * Remove a node
 *
 * Remove the first node it finds with value 'value' in l list
 *
 * @param int value
 * @return link
 * 
 */

link *removeLink (comp_graph_t *h, link *l, int value)
{

      if( !l ) return NULL;
      l->points=searchGraph(h,value);
	  if(l->points!=NULL)
	  {
		link *temp = l->next;  
		free(l);
		return temp;	
	  }
	  l->next = removeLink (h, l->next, value);
	  return l;
}		

/**
 * Alter node value
 *
 * Switch old value node for new value 
 *
 * @param int oldValue 
 * @param int newValue 
 * @return comp_graph_t
 *  
 */
comp_graph_t *alterGraphNodeValue(comp_graph_t *h, int oldValue, int newValue){
	h = searchGraph(h,oldValue);
	if(h!=NULL){
		h->item = newValue;
		return h;
	}
}		

