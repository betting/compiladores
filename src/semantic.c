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
			//printf("\nTYPE: %s -> %d", dictNode->token, dictNode->type);
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
		case 2: //function
			if((searchFunction(listFunction,dictNode->token)==NULL)&&(searchToken(listGlobal, dictNode->token)==NULL)) //nome da função não pode estar na lista de globais nem de funcão
			{
				listFunction = addFunction(dictNode->type, dictNode->token, listFunction,IKS_FUNCTION);
			}
			else
			{
				printf("Variavel global duplicada - linha %d\n", getLineNumber());
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

	if( ((type1 == IKS_INT)||(type1 == IKS_FLOAT)||(type1 == IKS_BOOL))&&((type2 == IKS_INT)||(type2 == IKS_FLOAT)||(type2 == IKS_BOOL)))
	{
		if(type1==IKS_INT && type2==IKS_INT) return IKS_INT;
		if(type1==IKS_FLOAT && type2==IKS_FLOAT) return IKS_FLOAT;
		if(type1==IKS_BOOL && type2==IKS_BOOL) return IKS_BOOL;
		if((type1==IKS_INT && type2==IKS_FLOAT) || (type2==IKS_INT && type1==IKS_FLOAT)) return IKS_FLOAT;
		if((type1==IKS_INT && type2==IKS_BOOL) || (type2==IKS_INT && type1==IKS_BOOL)) return IKS_INT;
		if((type1==IKS_BOOL && type2==IKS_FLOAT) || (type2==IKS_BOOL && type1==IKS_FLOAT))	return IKS_FLOAT;
	}
	else
	{
		if(((type1 == IKS_INT)||(type1 == IKS_FLOAT)||(type1 == IKS_BOOL)))	
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
/*
		case IKS_AST_ARIM_SUBTRACAO:
						stack_pointer->disc->node_type = inference(stack_pointer->disc->scc[0]->node_type,stack_pointer->disc->scc[1]->node_type);
						stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);break;

		case IKS_AST_ARIM_MULTIPLICACAO:
						stack_pointer->disc->node_type = inference(stack_pointer->disc->scc[0]->node_type,stack_pointer->disc->scc[1]->node_type);
						stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);break;

		case  IKS_AST_LOGICO_E:
						stack_pointer->disc->node_type = inference(stack_pointer->disc->scc[0]->node_type,stack_pointer->disc->scc[1]->node_type);
						stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);break;

		case IKS_AST_LOGICO_OU:
						stack_pointer->disc->node_type = inference(stack_pointer->disc->scc[0]->node_type,stack_pointer->disc->scc[1]->node_type);
						stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);break;

		case IKS_AST_ARIM_DIVISAO: 	
						stack_pointer->disc->node_type = inference(stack_pointer->disc->scc[0]->node_type,stack_pointer->disc->scc[1]->node_type);
						stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);break;

		case IKS_AST_RETURN:            if(func_type == stack_pointer->disc->scc[0]->node_type){
				
						stack_pointer->disc->node_type = stack_pointer->disc->scc[0]->node_type;
						stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);
						}
						else
						{
				                printf("Return com tipo diferente da funcao");
						exit(IKS_ERROR_WRONG_PAR_RETURN);
						}
								
						break;
		
		case IKS_AST_ARIM_INVERSAO:	stack_pointer->disc->node_type = stack_pointer->disc->scc[0]->node_type;
						stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);

						break;
		case IKS_OUTPUT:		
						
						break;

		case IKS_INPUT:		
						
						break;


		case IKS_AST_CHAMADA_DE_FUNCAO:
						break;


		case IKS_AST_IDENTIFICADOR: 	
						
						aux_list=list_lookup(global,stack_pointer->disc->symbol->text);

						if(aux_list!=NULL)
						{
								flag = 1;
								stack_pointer->disc->node_type = aux_list->type;
								stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);
								break;
						}

						else	
						{	
							aux_list=list_lookup(global_vet,stack_pointer->disc->symbol->text);

							if(aux_list!=NULL)
							{
								flag = 2;
								stack_pointer->disc->node_type = aux_list->type;
								stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);
								break;
							
							}
							else
							{	

								aux_list=list_lookup(local,stack_pointer->disc->symbol->text);

								if(aux_list!=NULL)
								{
								flag = 3;
								stack_pointer->disc->node_type = aux_list->type;
								stack_pointer->disc->size = var_size(stack_pointer->disc->node_type);
								break;
								}
								else
								{
									printf("Indentificador nao declarado");
									exit(IKS_ERROR_UNDECLARED);
		
								}
							}
						}
						break;
						

		case IKS_AST_LITERAL:
						stack_pointer->disc->node_type = stack_pointer->disc->symbol->type;
						stack_pointer->disc->size = var_size(stack_pointer->disc->symbol->type);break;

*/		
		}
		

		pointer= pointer->next;
		return  sPop(pointer, listFunction, listGlobal,listLocal,0);
	}
}
