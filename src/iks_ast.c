#include "iks_ast.h"
#include "comp_tree.h"


/** 
 * função que cria um novo nodo
 * 
 */

IKS_AST* criaIKS_AST(int tipo, comp_dict_item_t* simbolo, IKS_AST** filhos, int numFilhos) {
		IKS_AST *nodo = (AST*) malloc(sizeof(AST));
		nodo->tipo = tipo;
		nodo->numFilhos = numFilhos;
		nodo->simbolo = simbolo;
		nodo->filhos = filhos; 
		return nodo;
}

/** 
 * função que cria um novo nodo
 * 
 */
IKS_AST** criaNODOS(IKS_AST* f1, IKS_AST* f2, IKS_AST* f3, IKS_AST* f4, int numFilhos) {
	int index = 0;
	IKS_AST** nodos = (IKS_AST**)calloc(numFilhos, sizeof(IKS_AST*));
	criaNodo(nodos, f1, &index);
	criaNodo(nodos, f2, &index);
	criaNodo(nodos, f3, &index);
	criaNodo(nodos, f4, &index);
	return nodos;
}


/** 
 * função que cria nodo e incrementa o índice
 * 
 */
void criaNodo(IKS_AST** filhos,IKS_AST* filho, int* index) {
	if(filho != NULL) {
		filhos[(*index)] = filho;
		(*index)++;
	}

}

