/******************************************************
** comp_graph.h
** definições e constantes para as funções de grafo
******************************************************/
#include <stdio.h>
#include <malloc.h>

typedef struct _link
{
 struct node *points;
 struct _link *next;
} link;

typedef struct node
{
 int item,visit;
 struct _link *adjacent;
 struct node *next;
} comp_graph_t;

/**
 * Create a graph
 *
 * Initialize a graph with node and item
 *
 * @param *h comp_graph_t node
 * @param item int.
 * @return void
 */
void createGraph(comp_graph_t *h, int item);

/**
 * Search a node of given value in the graph
 *
 *
 * @param *h graph
 * @param p int
 * @return comp_graph_t
 */
comp_graph_t *searchGraph(comp_graph_t *h,int p);

/**
 * Connect a given node to the graph
 *
 *
 * @param *h graph node
 * @param *l link
 * @param int value
 * @return void
 */
void connectNodeGraph(comp_graph_t *h,  link *l, int value);

/**
 * Remove a given link of the graph
 *
 *
 * @param *h graph node
 * @param *l link
 * @param int value
 * @return void
 */
link *removeLink (comp_graph_t *h, link *l, int value);

/**
 * Edit the value of a given node of the graph
 *
 *
 * @param *h graph node
 * @param int old value
 * @param int new value
 * @return comp_graph_t node
 */
comp_graph_t *alterGraphNodeValue(comp_graph_t *h, int oldValue, int newValue);
