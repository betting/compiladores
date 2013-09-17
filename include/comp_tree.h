/******************************************************
** comp_tree.h
** definições e constantes para as funções de arvore
******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "comp_dict.h"

#ifndef __IKS_AST_H
#define __IKS_AST_H
#define IKS_AST_PROGRAMA             0
#define IKS_AST_FUNCAO               1
#define IKS_AST_IF_ELSE              2
#define IKS_AST_DO_WHILE             3
#define IKS_AST_WHILE_DO             4
#define IKS_AST_INPUT                5
#define IKS_AST_OUTPUT               6
#define IKS_AST_ATRIBUICAO           7
#define IKS_AST_RETURN               8
#define IKS_AST_BLOCO                9
#define IKS_AST_IDENTIFICADOR       10
#define IKS_AST_LITERAL             11
#define IKS_AST_ARIM_SOMA           12
#define IKS_AST_ARIM_SUBTRACAO      13
#define IKS_AST_ARIM_MULTIPLICACAO  14
#define IKS_AST_ARIM_DIVISAO        15
#define IKS_AST_ARIM_INVERSAO       16 // - (operador unário -)
#define IKS_AST_LOGICO_E            17 // &&
#define IKS_AST_LOGICO_OU           18 // ||
#define IKS_AST_LOGICO_COMP_DIF     19 // !=
#define IKS_AST_LOGICO_COMP_IGUAL   20 // ==
#define IKS_AST_LOGICO_COMP_LE      21 // <=
#define IKS_AST_LOGICO_COMP_GE      22 // >=
#define IKS_AST_LOGICO_COMP_L       23 // <
#define IKS_AST_LOGICO_COMP_G       24 // >
#define IKS_AST_LOGICO_COMP_NEGACAO 25 // !
#define IKS_AST_VETOR_INDEXADO      26 // para var[exp] quando o índice exp é acessado no vetor var
#define IKS_AST_CHAMADA_DE_FUNCAO   27
#endif

//definicao da estrutura do elemento LISTA
typedef struct _tree
{
	int type;
	comp_dict_item_t* symbol;
	struct _tree *child,*sibling;
} comp_tree_t;

/* prototipos das funções */ 
comp_tree_t* search(comp_tree_t* root,int data);
comp_tree_t* initTree(comp_tree_t* root,int data[]);
comp_tree_t* createNode(int type, comp_dict_item_t* symbol);
void insertChild(comp_tree_t* parent, comp_tree_t* child);
void addSiblings(comp_tree_t* first, comp_tree_t* newNode);
