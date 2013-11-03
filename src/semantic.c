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

FILE *logFile;

/**
 *  Initiate Stack
 *  
 * 
 */
STACK* initStack(void)
{
	STACK* pilha = malloc(sizeof(STACK*));
	pilha = NULL;
	return pilha;
}

void printStack(STACK* stack_l)
{
   while( stack_l != NULL)
   {
      // Checking if there's a token available
      if (stack_l->disc->symbol != NULL)
      {
         printf("%s\n", stack_l->disc->symbol->token);
      }
      else
      {
         printf("Op Type: %d\n", stack_l->disc->type);
      }

      stack_l = stack_l->next;
   }
}

STACK* sPush(STACK* pointer, comp_tree_t* nodoAST)
{
	STACK* new = malloc(sizeof(STACK*));
	new->disc = nodoAST;
	new->next = NULL;
	new->previous = NULL;

   pointer = addElementStack(pointer, new);

	return pointer;
}


STACK* addElementStack(STACK* S, STACK* newElement)
{
   STACK* pilha_aux = malloc(sizeof(STACK*));

   if (S == NULL)
   {
      pilha_aux = newElement;
   }
   else
   {
      pilha_aux = newElement;
      pilha_aux->next = S;
      pilha_aux->previous = NULL;
   }

   S = pilha_aux;
   return S;
}

int sizeDeclarations(int type)
{
	switch(type)
	{
      case IKS_SIMBOLO_LITERAL_INT: return 4; break;
      case IKS_SIMBOLO_LITERAL_FLOAT: return 8; break;
      case IKS_SIMBOLO_LITERAL_BOOL: return 1; break;
      case IKS_SIMBOLO_LITERAL_CHAR: return 1; break;
      case IKS_SIMBOLO_LITERAL_STRING: return 1; break;
	}
}


int insertDeclarations(comp_dict_item_t* dictNode, int escopo)
{
	/*
	 * Verificar se declaração já está na lista:
	 * Não -> inserir nodo
	 * Sim -> retornar erro
	 */
   comp_list_t *lastFunctionNameItem;
   comp_list_t *localList;
	switch(escopo)
	{
		case IKS_LOCAL: //local
         lastFunctionNameItem = getLastItemList(listFunctions);
         localList = getLocalList(declarationList, lastFunctionNameItem);
        
			if(searchToken(localList, dictNode->token) == NULL)
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
		default:
				printf("Funcao não declarada - linha %d\n", getLineNumber());
				return IKS_ERROR_UNDECLARED;
			break;
	}
}


/**
 * Check Declarations.
 * 
 * Check all variable and function declarations.
 *
 * @param *program  All the program in stack
 * @param *declarationList List of all variable and function declared
 * @return The return code (sucess or error)
 */
int checkDeclarations(STACK* program, comp_list_t* declarationList)
{
   int retorno;
   STACK* stack_l = program;
   /*
   while (stack->next != NULL)
   {
      // Checking if actual symbol is a global declaration 
      if (stack->disc->symbol->type == IKS_SIMBOLO_IDENTIFICADOR)
      {
         retorno = verifyGlobalDeclarations(stack->disc->symbol->token, declarationList);
         if (retorno == FALSE)
         {
            return FALSE;
         }
      }


      stack = stack->next;
   }
   */
   while (stack_l->next != NULL)
   {
      //if (stack_l->disc->symbol->type == IKS_SIMBOLO_IDENTIFICADOR)
      if (stack_l->disc->symbol != NULL)
      {
			retorno = searchTypeToken(declarationList,stack_l->disc->symbol->token);
			printf("\n\nToken %s - RETORNO %d",stack_l->disc->symbol->token, retorno);
	   }
	   stack_l = stack_l->next;
   }
   return retorno;
/*
   // Checking if there are variable/vector declariation or function definition.
   if(stack->disc->symbol->type == IKS_AST_FUNCAO || stack->disc->symbol->type == IKS_AST_VETOR_INDEXADO)
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
*/
//return 1;
}

int verifyGlobalDeclarations(char* token, comp_list_t* declarationList)
{
   while (declarationList->next != NULL)
   {
      if ((strcmp(declarationList->nomeVar, token) == 0)
            && (declarationList->tipoGlobal == IKS_GLOBAL_VAR))
      {
         return TRUE;
      }
      declarationList = declarationList->next;
   }

   return FALSE; 
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



int coertion(int type1, int type2)
{
	
        if(type1 == type2) return type1;

		//erros de coerção
        if(type2 == IKS_SIMBOLO_LITERAL_STRING) exit(IKS_ERROR_STRING_TO_X);
        if(type2 == IKS_SIMBOLO_LITERAL_CHAR) exit(IKS_ERROR_CHAR_TO_X);
		if((type1 == IKS_SIMBOLO_LITERAL_STRING) || (type1 == IKS_SIMBOLO_LITERAL_CHAR )) exit(IKS_ERROR_WRONG_TYPE); 	

        if(type1 == IKS_SIMBOLO_LITERAL_INT && (type2 == IKS_SIMBOLO_LITERAL_FLOAT||type2 == IKS_SIMBOLO_LITERAL_BOOL)) return IKS_SIMBOLO_LITERAL_INT;  
        if((type1 == IKS_SIMBOLO_LITERAL_FLOAT) && ((type2 == IKS_SIMBOLO_LITERAL_INT)||(type2 == IKS_SIMBOLO_LITERAL_BOOL))) return IKS_SIMBOLO_LITERAL_FLOAT;
        if((type1 == IKS_SIMBOLO_LITERAL_BOOL) && (type2 == IKS_SIMBOLO_LITERAL_FLOAT || type2 == IKS_SIMBOLO_LITERAL_INT)) return IKS_SIMBOLO_LITERAL_BOOL;
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
      ( (type1 == IKS_SIMBOLO_LITERAL_INT)
            || (type1 == IKS_SIMBOLO_LITERAL_FLOAT)
            || (type1 == IKS_SIMBOLO_LITERAL_BOOL)
      ) && (
        (type2 == IKS_SIMBOLO_LITERAL_INT)
            || (type2 == IKS_SIMBOLO_LITERAL_FLOAT)
            || (type2 == IKS_SIMBOLO_LITERAL_BOOL)
        )
     )
	{
      // Int
      if(type1 == IKS_SIMBOLO_LITERAL_INT && type2 == IKS_SIMBOLO_LITERAL_INT) {printf("\n tipoINT: %d",IKS_SIMBOLO_LITERAL_INT); return IKS_SIMBOLO_LITERAL_INT;};
      if((type1 == IKS_SIMBOLO_LITERAL_INT && type2 == IKS_SIMBOLO_LITERAL_BOOL) || (type1 == IKS_SIMBOLO_LITERAL_BOOL && type2 == IKS_SIMBOLO_LITERAL_INT)) {printf("\n tipoINT: %d",IKS_SIMBOLO_LITERAL_INT); return IKS_SIMBOLO_LITERAL_INT;};

      // Float
      if(type1 == IKS_SIMBOLO_LITERAL_FLOAT && type2 == IKS_SIMBOLO_LITERAL_FLOAT) {printf("\n tipoFLOAT: %d",IKS_SIMBOLO_LITERAL_FLOAT); return IKS_SIMBOLO_LITERAL_FLOAT; };
      if((type1 == IKS_SIMBOLO_LITERAL_INT && type2 == IKS_SIMBOLO_LITERAL_FLOAT) || (type1 == IKS_SIMBOLO_LITERAL_FLOAT && type2 == IKS_SIMBOLO_LITERAL_INT)) {printf("\n tipoFLOAT: %d",IKS_SIMBOLO_LITERAL_FLOAT); return IKS_SIMBOLO_LITERAL_FLOAT; };
      if((type1 == IKS_SIMBOLO_LITERAL_BOOL && type2 == IKS_SIMBOLO_LITERAL_FLOAT) || (type1 == IKS_SIMBOLO_LITERAL_FLOAT && type2 == IKS_SIMBOLO_LITERAL_BOOL))	{printf("\n tipoFLOAT: %d",IKS_SIMBOLO_LITERAL_FLOAT); return IKS_SIMBOLO_LITERAL_FLOAT; };

      // Bool
      if(type1 == IKS_SIMBOLO_LITERAL_BOOL && type2 == IKS_SIMBOLO_LITERAL_BOOL) {printf("\n tipoFLOAT: %d",IKS_SIMBOLO_LITERAL_BOOL); return IKS_SIMBOLO_LITERAL_BOOL; };
	}
	else
	{
      if(((type1 == IKS_SIMBOLO_LITERAL_INT) || (type1 == IKS_SIMBOLO_LITERAL_FLOAT) || (type1 == IKS_SIMBOLO_LITERAL_BOOL)) && ((type2 != IKS_SIMBOLO_LITERAL_INT) || (type2 != IKS_SIMBOLO_LITERAL_FLOAT) || (type2 != IKS_SIMBOLO_LITERAL_BOOL)))
      {
         if(type2 == IKS_SIMBOLO_LITERAL_CHAR) exit(IKS_ERROR_CHAR_TO_X);
         if(type2 == IKS_SIMBOLO_LITERAL_STRING) exit(IKS_ERROR_STRING_TO_X);
      }
      else 
      if((type1 == IKS_SIMBOLO_LITERAL_STRING)||(type1 == IKS_SIMBOLO_LITERAL_CHAR)) exit(IKS_ERROR_WRONG_TYPE);

   }
}


void sPop(STACK* pointer, comp_list_t* function, comp_list_t* local, int func_type)
{

	printf("\n$$$$$$$ TIPO DA FUNC = %d  ======\n", pointer->disc->type);
	int flag = 0;	// 1- global var ----- 2- global vector ------- 3- local var
	comp_list_t* aux_list;
    comp_list_t *lastFunctionNameItem;
    comp_list_t *localList;
    STACK* aux_stack;
    
	int aux_type = pointer->disc->type;
	int aux_type1, aux_type2;
	
	printf("\nANTES DO IF");
	if(pointer!=NULL)
	{ 
		printf("\nANTES DO SWITCH");

		switch(pointer->disc->type)
		{
			case IKS_AST_ARIM_SUBTRACAO:
            case IKS_AST_ARIM_MULTIPLICACAO:
            case IKS_AST_LOGICO_E:
            case IKS_AST_LOGICO_OU:
            case IKS_AST_ARIM_DIVISAO:
            case IKS_AST_LOGICO_COMP_DIF:
            case IKS_AST_LOGICO_COMP_IGUAL:
            case IKS_AST_LOGICO_COMP_LE:
            case IKS_AST_LOGICO_COMP_GE:
            case IKS_AST_LOGICO_COMP_L:
            case IKS_AST_LOGICO_COMP_G: 
			case IKS_AST_ARIM_SOMA:
							
							printf("\nEntrei em alguma operação !");
							if(pointer->disc->symbol != NULL) printf("\nSYMBOL IS NOT NULL");
							if(pointer->disc->child != NULL)
							{
								printf("\nCHILD IS NOT NULL");
								printf("\nSYMBOL - TOKEN: %s\n",pointer->disc->child->symbol->token);
							}
							if(pointer->disc->child->sibling != NULL)
							{
								printf("\nSIBLING IS NOT NULL");
								printf("\nSYMBOL - TOKEN: %s\n",pointer->disc->child->sibling->symbol->token);
							}
							/*
							aux_stack = pointer;
							aux_stack = aux_stack->previous;
							
							//AQUI TEM QUE PEGAR O TIPO DA DECLARAÇÃO
							aux_type2 = aux_stack->disc->symbol->type;
							
							printf("\nAUX->SYMBOL - TOKEN: %s - TIPO: %d\n",aux_stack->disc->symbol->token, aux_type2);
							aux_stack = aux_stack->previous;

							//AQUI TEM QUE PEGAR O TIPO DA DECLARAÇÃO
							aux_type1 = aux_stack->disc->type;
							printf("\nAUX->SYMBOL - TOKEN: %s - TIPO: %d\n",aux_stack->disc->symbol->token, aux_type1);
								
														
							pointer->disc->node_type = inference(aux_type1,aux_type2);
							*/
							break;
	
			/*case IKS_AST_ARIM_SUBTRACAO:
							pointer->disc->type = inference(pointer->disc->child->type,pointer->disc->sibling->type);
							pointer->disc->size = sizeDeclarations(pointer->disc->type);
							printf("\n\tSUBTRAÇÃO type: %d - size: %d",pointer->disc->type,pointer->disc->size);
							break;
			*/	
			case IKS_AST_IDENTIFICADOR:
						printf("\n\t\t CHEGOU NO IDENTIFICADOR");
						//search in function list 
						/*lastFunctionNameItem = getLastItemList(listFunctions);
						localList = getLocalList(declarationList, lastFunctionNameItem);
						aux_list=searchToken(declarationList,pointer->disc->symbol->token);
						
						if(aux_list!=NULL)
						{
								//printf("\nIDENTX");
								pointer->disc->type = aux_list->tipoVar;
								pointer->disc->size = sizeDeclarations(pointer->disc->type);
								printf("\n\tIDENTIFICADOR type: %d - size: %d",pointer->disc->type,pointer->disc->size);
								break;
						}

						else	
						{	//search in global list
							aux_list=searchToken(declarationList,pointer->disc->symbol->token);

							if(aux_list!=NULL)
							{
								printf("\nIDENTY");
								pointer->disc->type = aux_list->tipoVar;
								pointer->disc->size = sizeDeclarations(pointer->disc->type);
								break;
							
							}
							else
							{	
								//search in local list
								aux_list=searchToken(localList,pointer->disc->symbol->token);

								if(aux_list!=NULL)
								{
									printf("\nIDENTZ");
									pointer->disc->type = aux_list->tipoVar;
									pointer->disc->size = sizeDeclarations(pointer->disc->type);
								break;
								}
								else
								{
									printf("\nIDENTW");
									printf("\nVariável não declarada");								
									exit(IKS_ERROR_UNDECLARED);
								}
							}
						}*/
						break;
						
			case IKS_AST_LITERAL:
						printf("\nIKS_AST_LITERAL");
			
						/*pointer->disc->type = aux_list->tipoVar;
						pointer->disc->size = sizeDeclarations(pointer->disc->type);
						printf("\n\tLITERAL type: %d - size: %d",pointer->disc->type,pointer->disc->size);*/
						break;
						
			case IKS_AST_ATRIBUICAO:
						printf("\nIKS_AST_ATRIBUICAO");
						break;

			case IKS_AST_INPUT: 
						printf("\nIKS_AST_INPUT");
						break;

			case IKS_AST_OUTPUT:
						printf("\nIKS_AST_INPUT");
						break;

			case IKS_AST_RETURN:
						printf("\nIKS_AST_INPUT");
						break;
			default:
						printf("\nCAIU NO DEFAULT");
						printf("\nTipo: %d",pointer->disc->type);
						break;
				
		 
		}
			pointer = pointer->next;
			if(pointer!=NULL)
				return  sPop(pointer, listFunctions, declarationList,0);
	}
	else
	{
		printf("\nPOINTER NULL");
	}
	
	
}
