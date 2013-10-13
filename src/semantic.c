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

STACK* invert(STACK* stack)
{
	while(stack->previous != NULL)
		stack = stack->previous;

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

L_function* searchFunction(L_function* list, char* text)
{
		L_function* aux = list;

		while(aux != NULL)
		{	
			if(strcmp(aux->nameFunction, text) == 0)
				return aux;
			aux = aux->next;
		}

		return NULL;
}

L_function* addFunction(int type, char *nameFunction, L_function* node, int tipoGlobal)
{
	
	L_function* new;
	new = (L_function *)malloc(sizeof(L_function));
	new->type = type;
	new->nameFunction = nameFunction;
	new->next = NULL;

    node = addNodeFunction(node, new);

	return node;
}

L_function* addNodeFunction(L_function* list, L_function* node)
{
   if (list == NULL)
   {
      list = node;
   }
   else if (list->next == NULL)
   {
      list->next = node;
   }
   else
   {
      addNodeFunction(list->next, node);
   }
   return list;
}

int sizeDeclarations(int type)
{
	switch(type)
	{
      case IKS_INT: return 4; break;
      case IKS_FLOAT: return 8; break;
      case IKS_BOOL: return 1; break;
      case IKS_CHAR: return 1; break;
      case IKS_STRING: return 1; break;
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
		case IKS_LOCAL: //local

         
			if(searchToken(declarationList, dictNode->token) == NULL)
			{
				declarationList = addItem(dictNode->type, dictNode->token, declarationList, IKS_LOCAL);
			}
			else
			{
				printf("Variavel local duplicada - linha %d\n", getLineNumber());
				return IKS_ERROR_DECLARED;
			}
			break;
			
		case IKS_GLOBAL_VAR: //variavel global
			if(searchToken(declarationList, dictNode->token) == NULL)
			{
				declarationList = addItem(dictNode->type, dictNode->token, declarationList, IKS_GLOBAL_VAR);
			}
			else
			{
				printf("Variavel global duplicada - linha %d\n", getLineNumber());
				return IKS_ERROR_DECLARED;
			}		
		
			break;
			
		case IKS_GLOBAL_VET: //vetor global
			if(searchToken(declarationList, dictNode->token) == NULL)
			{
				declarationList = addItem(dictNode->type, dictNode->token, declarationList, IKS_GLOBAL_VET);
			}
			else
			{
				printf("Variavel (vetor) global duplicado - linha %d\n", getLineNumber());
				return IKS_ERROR_DECLARED;
			}	
			break;

		case IKS_FUNCTION: //function
         // Function name could not be in global or function lists.
			if((searchToken(listFunctions, dictNode->token) == NULL)
            && (searchToken(declarationList, dictNode->token) == NULL)) 
			{
				listFunctions = addItem(dictNode->type, dictNode->token, listFunctions, IKS_FUNCTION);
				declarationList = addItem(dictNode->type, dictNode->token, declarationList, IKS_FUNCTION);
			}
			else
			{
				printf("Funcao com nome duplicado - linha %d\n", getLineNumber());
				return IKS_ERROR_DECLARED;
			}
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
//	else if(root->type == IKS_AST_CHAMADA_DE_FUNCAO){}        // Ter certeza que a função está sendo declarada como função */

}



/*
 * Regras de inferência
 * @param type1
 * @param type2 
 */
int inference(int type1, int type2)
{
   /*
    * Type 01: Int/Float/Bool
    * Type 02: Int/Float/Bool
    */
	if(
      ( (type1 == IKS_INT)
            || (type1 == IKS_FLOAT)
            || (type1 == IKS_BOOL)
      ) && (
        (type2 == IKS_INT)
            || (type2 == IKS_FLOAT)
            || (type2 == IKS_BOOL)
        )
     )
	{
      // Int
      if(type1 == IKS_INT && type2 == IKS_INT) return IKS_INT;
      if((type1 == IKS_INT && type2 == IKS_BOOL) || (type1 == IKS_BOOL && type2 == IKS_INT)) return IKS_INT;

      // Float
      if(type1 == IKS_FLOAT && type2 == IKS_FLOAT) return IKS_FLOAT;
      if((type1 == IKS_INT && type2 == IKS_FLOAT) || (type1 == IKS_FLOAT && type2 == IKS_INT)) return IKS_FLOAT;
      if((type1 == IKS_BOOL && type2 == IKS_FLOAT) || (type1 == IKS_FLOAT && type2 == IKS_BOOL))	return IKS_FLOAT;

      // Bool
      if(type1 == IKS_BOOL && type2 == IKS_BOOL) return IKS_BOOL;
	}
	else
	{
      if((type1 == IKS_INT)
            || (type1 == IKS_FLOAT)
            || (type1 == IKS_BOOL))
      {
         return type1;
      }
      else
      {	
         return type2;
      }
   }
}


void sPop(STACK* pointer,L_function* function,comp_list_t* global, comp_list_t* local,int func_type)
{

	printf("$$$$$$$ TIPO DA FUNC = %d  $$$$$$$$$$", func_type);
	int flag = 0;	// 1- global var ----- 2- global vector ------- 3- local var
	comp_list_t* aux_list;

	if(pointer!=NULL){ 
		switch(pointer->disc->type){
		
		case IKS_AST_ARIM_SOMA:
						pointer->disc->type = inference(pointer->disc->child->type,pointer->disc->sibling->type);
						printf("\nInferencia: %d",pointer->disc->type);
						pointer->disc->size = sizeDeclarations(pointer->disc->type);
						printf("\nSIZE: %d",pointer->disc->type);						
						break;
	
		}
		

		pointer= pointer->next;
//		return  sPop(pointer, listFunctions, listGlobal,declarations,0);
	}
}
