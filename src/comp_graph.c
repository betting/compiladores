#include "comp_graph.h"


//---------------------------------------------------
//                Create Graph
//---------------------------------------------------

void createGraph(struct node *h, int item)
{
	h->item = item;
	h->visit = 0;
	//aloca o espaço para o próximo
	h->next = (struct node *)malloc(sizeof(struct node));
}

node *searchGraph(struct node *h,int p)
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

