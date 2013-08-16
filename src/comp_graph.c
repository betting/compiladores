#include "comp_graph.h"


//---------------------------------------------------
//                Create Graph
//---------------------------------------------------

void create(struct node *h, int item)
{
	h->item = item;
	h->visit=0;
	h->next=(node*)malloc(sizeof(node));
	create(h->next);
}
