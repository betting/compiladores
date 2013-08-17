/******************************************************
** comp_graph.h
** definições e constantes para as funções de grafo
******************************************************/
#ifndef _comp_graph_h
#define _comp_graph_h
#include <stdio.h>
#include <malloc.h>

//definicao da estrutura para o elemento GRAFO
//ligações
struct link
{
 struct node *points;
 struct link *next;
};
//nós
typedef struct node
{
 int item,visit;
 struct link *adjacent;
 struct comp_graph_t *next;
} comp_graph_t;


//prototipos das funcoes
void createGraph(struct node *h, int item);
comp_graph_t *searchGraph(comp_graph_t *h,int p);
void getAdjacent(struct node *h,link *l)




#endif
