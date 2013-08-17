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

void getAdjacent(node *h,link *l)
{
  if(h==NULL)return;
  if(l==NULL)
  {	
	l =(struct link *)malloc(sizeof(struct link));
	//l=(link*)malloc(sizeof(link));
	h->adjacent=l;
  }
  //h->item é adjacente? se sim... senão...
  if(????)
  {

    int t;
    //t = valor
    l->points=search(comp_graph_t,t);
    l->next=(link*)malloc(sizeof(link));
    getAdjacent(h,l->next);
   }
  else
  {
    l->next=NULL;
    getAdjacent(h->next,l->next);
  }
}
