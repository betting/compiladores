

#include <stdlib.h>
#include "stack.h"
#include "comp_list.h"
#include "iks_ast.h"
#include "semantic.h"

STACK2* initStack2()
{
	STACK2* stack_l = malloc(sizeof(STACK2*));
	stack_l = NULL;
	return stack_l;
}

/**
 * Adding new node to a list
 *
 * Add new element in the list, with the value specified.
 *
 * @param *list List with all elements.
 * @param *node New node to be added.
 * @return The list with the new element.
 */
STACK2* addStack(STACK2* stack_l, STACK2* node)
{
   if (stack_l == NULL)
   {
      stack_l = node;
   }
   else if (stack_l->next == NULL)
   {
      stack_l->next = node;
   }
   else
   {
      addStack(stack_l->next, node);
   }
   return stack_l;
}

STACK2* addItemStack(int tipoVar, char *nomeVar, int tipoGlobal, STACK2* node)
{
	
	STACK2* new;
	new = (STACK2 *)malloc(sizeof(STACK2));
	new->tipoVar = tipoVar;
	new->nomeVar = nomeVar;
	new->type = tipoGlobal;
	new->next = NULL;
	new->size = sizeDeclarations(tipoVar);
    
    node = addStack(node, new);

	return node;
}

//insere no topo da pilha
STACK2* listToStack(comp_list_t* list)
{
	STACK2* new = initStack2();
	STACK2* aux = initStack2();
	//comp_list_t* list = (comp_list_t *)malloc(sizeof(comp_list_t));

	//list = list_aux;

	
	int aux_tipoGlobal, aux_tipoVar;
	char *aux_nomeVar;
	
	//printf("\n\nPUSH:");
	
		if(new == NULL)
		{
			//new = initStack2();
			aux_nomeVar = list->nomeVar;
			aux_tipoVar = list->tipoVar;
			aux_tipoGlobal = list->tipoGlobal;	
			
			printf("\nNOME_VAR => %s", aux_nomeVar);
			printf("\nTIPO_VAR => %d", aux_tipoVar);
			printf("\nGLOBAL_VAR => %d", aux_tipoGlobal);

			aux = addItemStack(aux_tipoVar,aux_nomeVar,aux_tipoGlobal,aux);	
			aux->next = NULL;

			list = list->next;

			new = aux;
		}

	while(list != NULL)
	{
		aux_nomeVar = list->nomeVar;
		aux_tipoVar = list->tipoVar;
		aux_tipoGlobal = list->tipoGlobal;	
			
		printf("\nNOME_VAR => %s", aux_nomeVar);
		printf("\nTIPO_VAR => %d", aux_tipoVar);
		printf("\nGLOBAL_VAR => %d", aux_tipoGlobal);
			
		aux = addItemStack(aux_tipoVar,aux_nomeVar,aux_tipoGlobal,aux);	
		list = list->next;
	
	}

	return new;
}


void printStack2(STACK2* stack_l)
{
   printf("\n\nSTACK2");

   while( stack_l != NULL)
   {
	  printf("\n(GLOBAL)STACK->TYPE = %d", stack_l->type);
	  printf("\nSTACK->tipoVar = %d", stack_l->tipoVar);
	  printf("\nSTACK->nomeVar = %s", stack_l->nomeVar);
	  printf("\nSTACK->size = %d", stack_l->size);
	  stack_l = stack_l->next;
   }
   
}


/*
Pessoal, seguinte.
Eu não vou conseguir fazer sozinho a etapa 4. 
Eu não sou um programador C, mas to tentando e outra vou viajar no final de semana, 
portanto vamos acelerar isso agora e não deixar pro dia 28 apenas.

Abaixo as definições pra encontrar os erros:
TEM QUE DEFINIR QUANDO É SYMBOL ANTES
TEM QUE DEFINIR QUANDO É VARIABLE ANTES
TEM QUE DEFINIR QUANDO É VECTOR ANTES
PARAMETRO = variável de função

if (stack->type é IKS_SYMBOL) OU (stack->type é IKS_VARIABLE)
{
		if symbol->tipoGlobal é VETOR
		        retorna IKS_ERROR_VARIABLE
		else if symbol->tipoGlobal é FUNÇÃO
		        retorna IKS_ERROR_VARIABLE
		else if (symbol->tipoGlobal é diferente de VARIABLE) E (symbol->tipoGlobal é diferente de PARAMETRO)
		        retorna IKS_ERROR_UNDECLARED
}

if (stack->type é IKS_SYMBOL_VECTOR) OU (stack->type é IKS_ATRIBUTO_VECTOR)
{
		if (symbol->tipoGlobal é VARIABLE) OU (symbol->tipoGlobal é PARAMETRO)
		        retorna IKS_ERROR_VECTOR
		else if symbol->tipoGlobal é FUNÇÃO
		        retorna IKS_ERROR_VECTOR
		else if (symbol->tipoGlobal é diferente de IKS_SYMBOL_VECTOR)
		        retorna IKS_ERROR_UNDECLARED
}

if (stack->type é CHAMADA_DE_FUNÇÃO)
{
		if (symbol->tipoGlobal é VARIABLE) OU (symbol->tipoGlobal é PARAMETRO)
		        retorna IKS_ERROR_FUNCTION
		else if if symbol->tipoGlobal é VETOR
		        retorna IKS_ERROR_FUNCTION
		else if symbol->tipoGlobal é diferente de FUNÇÃO
		        retorna IKS_ERROR_FUNCTION
		else if symbol->tipoGlobal é  FUNÇÃO
		         FAZER VERIFICAÇÃO DA CHAMADA DOS PARAMETROS
}

*/
