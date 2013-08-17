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
struct node
{
 int item,visit;
 struct link *adjacent;
 struct node *next;
}*head;

//prototipos das funcoes
void createGraph(struct node *h, int item);
node *searchGraph(struct node *h,int p)
//void getAdjacent(struct node *h,link *l)




#endif
