/**
 * @file semantic.c
 * @version 1.0
 *
 * @section DESCRIPTION
 * Semantic Analisys
 * Perform a semantic analisys in a code provided by the user
 */

#include <stdlib.h>
#include "comp_tree.h"
#include "iks_ast.h"
#include "semantic.h"

/**
 *  Initiate Stack
 *  
 * 
 */
STACK* initStack(void)
{
	STACK* stack = malloc(sizeof(STACK*));
	stack = NULL;
	return stack;
}

STACK* sPush(STACK* pointer, comp_tree_t* nodoAST)
{

	STACK* new = malloc(sizeof(STACK*));
	new->disc = nodoAST;
	new->next = NULL;
	new->previous = NULL;


	if(pointer != NULL){ 
		new->previous = pointer;
		pointer->next = new;
	}
		
	pointer = new;

	return pointer;
}

int insertGlobalDeclarations()
{
	/*
	 * Verificar se declaração já está na lista:
	 * Não -> inserir nodo
	 * Sim -> retornar erro
	 * 
	 * TODO LIST:
	 * 
	 * 
	 */

}

int sizeDeclarations(int type)
{
	switch(type)
	{
		case IKS_INT:    return 4;break;
		case IKS_FLOAT:  return 8;break;
		case IKS_BOOL:   return 1;break;
		case IKS_CHAR:   return 1;break;
		case IKS_STRING: return 1;break;
	}
}


int insertDeclarations(comp_dict_item_t* dictNode, int escopo)
{
	/*
	 * Verificar se declaração já está na lista:
	 * Não -> inserir nodo
	 * Sim -> retornar erro
	 */
	switch(escopo)
	{
		case -1: //local 
			//printf("LOCAL DECLARATIONS");
			printf("\nTYPE: %s -> %d", dictNode->token, dictNode->type);
			if(searchToken(listLocal, dictNode->token)==NULL)
			{
				listLocal = addItem(dictNode->type, dictNode->token, listLocal,IKS_LOCAL);
			}
			else
			{
				printf("Variavel local duplicada - linha %d\n", getLineNumber());
				return IKS_ERROR_DECLARED;
			}
			break;
			
		case 0: //variavel global
			if(searchToken(listGlobal, dictNode->token)==NULL)
			{
				listGlobal = addItem(dictNode->type, dictNode->token, listGlobal,IKS_GLOBAL_VAR);
			}
			else
			{
				printf("Variavel global duplicada - linha %d\n", getLineNumber());
				return IKS_ERROR_DECLARED;
			}		
		
			break;
			
		case 1: //vetor global
			if(searchToken(listGlobal, dictNode->token)==NULL)
			{
				listGlobal = addItem(dictNode->type, dictNode->token, listGlobal,IKS_GLOBAL_VET);
			}
			else
			{
				printf("Variavel global duplicada - linha %d\n", getLineNumber());
				return IKS_ERROR_DECLARED;
			}	
			break;
		case 2: 
			break;
	}
}


/**
 * Check Declarations.
 * 
 * Check all variable and function declarations.
 *
 * @param *root   The tree with all elements found
 * @return The return code (sucess or error)
 */
int checkDeclarations(comp_tree_t *root)
{
   // Checking if there are variable/vector declariation or function definition.
   if(root->type == IKS_AST_FUNCAO || root->type == IKS_AST_VETOR_INDEXADO)
   {
      if(root->symbol == 0)
      {
         // Identifier undeclared
         return IKS_ERROR_UNDECLARED;
      }
      else
      {
         if (root->symbol->type != IKS_AST_IDENTIFICADOR)
         {
            // Symbol already definied
            return IKS_ERROR_DECLARED;
         }
	 else
	 { //definir qual o tipo do simbolo foi declarado: variável, vetor, função
		
		
	 }
      }
   }
}

/**
 * Check Utilization.
 * 
 * Check all variable and function utilization.
 *
 * @param *root   The tree with all elements found
 * @return The return code (sucess or error)
 */

int checkUtilization(comp_tree_t *root)
{
       if(root == 0) return;
        
        int i;

        // Ter certeza que a variável está sendo declarada como variável
/*        if(root->type == IKS_AST_SYMBOL || root->type == IKS_AST_VAR){
                if (root->symbol->type == ) 
		{ //se type igual a VETOR
			return IKS_ERROR_VARIABLE; //identificador deve ser utilizado como variável
                } 
		else if(root->symbol->type == ) 
		{
			return IKS_ERROR_VARIABLE; //identificador deve ser utilizado como variável
                } 
		else if (root->symbol->type != )
		{
			return IKS_ERROR_UNDECLARED;
                }
        }
//	else if(){}        // Ter certeza que o vetor está sendo declarado como vetor
//	else if(root->type == IKS_AST_CHAMADA_DE_FUNCAO){}        // Ter certeza que a função está sendo declarada como função

*/
}


