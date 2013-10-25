

#include <stdlib.h>
#include "stack.h"
#include "comp_list.h"
#include "iks_ast.h"
#include "semantic.h"

STACK2* initStack2()
{
	STACK2* stack_l = malloc(sizeof(STACK2*));
	stack_l = NULL;
	stack_l->disc->nomeVar = NULL;
	stack_l->disc->tipoVar = 0;
	stack_l->type = 99;
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

//insere no topo da pilha
STACK2* push_s(comp_list_t* list)
{
	comp_list_t* list_aux;

	STACK2* new;
	new = initStack2();
	
	int aux_tipoGlobal, aux_tipoVar;
	char *aux_nomeVar;
	
	//printf("\n\nPUSH:");
	/*
	while(list != NULL)
	{
		aux_nomeVar = list->nomeVar;
		aux_tipoVar = list->tipoVar;
		aux_tipoGlobal = list->tipoGlobal;	
		
		printf("\nNOME_VAR => %s", aux_nomeVar);
		printf("\nTIPO_VAR => %d", aux_tipoVar);
		printf("\nGLOBAL_VAR => %d", aux_tipoGlobal);
		
		new->disc->nomeVar = aux_nomeVar;
		new->disc->tipoVar = aux_tipoVar;
		new->type = aux_tipoGlobal;
		
		new->size = sizeDeclarations(list->tipoVar);
		
		new->next = initStack2();
		new = new->next;
		list = list->next;	
	}
	*/
	
	
	//node = addStack(node, new);

	return new;
}



void printStack2(STACK2* stack_l)
{
   printf("\n\nSTACK2");

   while( stack_l != NULL)
   {
	  printf("\n(GLOBAL)STACK->TYPE = %d", stack_l->type);
	  printf("\nSTACK->disc->tipoVar = %d", stack_l->disc->tipoVar);
	  printf("\nSTACK->disc->nomeVar = %s\n", stack_l->disc->nomeVar);
	  stack_l = stack_l->next;
      //printf("%d \t %s \t %d \n", stack_l->disc->type, stack_l->disc->symbol->token, stack_l->disc->size);
      //stack_l = stack_l->next;
   }
 /*  
   printf("\n\nNO SYMBOL \n\t Tipo de Dados \t Variável \t Tamanho \n");
 
   while( stack_l != NULL)
   {
      printf("%d \t %d \t %s \n", stack_l->disc->type, stack_l->disc->symbol->token, stack_l->disc->symbol->token);
      list = list->next;
   }
   */
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
