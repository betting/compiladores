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
comp_tree_t *variableName;
comp_tree_t *data;
comp_tree_t *dataChild;

int variableTypeGlobal;
int variableTypeLocal;
int variableTypeVector;
int variableTypeFunction;

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
         if(stack_l->disc->type == IKS_AST_INPUT) printf("IKS_AST_INPUT\n\n");
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
				exit(IKS_ERROR_DECLARED);
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
				exit(IKS_ERROR_DECLARED);
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
				exit(IKS_ERROR_DECLARED);
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
				exit(IKS_ERROR_DECLARED);
			}
			break;
		default:
				printf("Funcao não declarada - linha %d\n", getLineNumber());
				exit(IKS_ERROR_UNDECLARED);
			break;
	}
}

int coertion(int type1, int type2)
{
   if(type1 == type2)
   {
      return type1;
   }

   // Erros de coerção
   if(type2 == IKS_SIMBOLO_LITERAL_STRING)
   {
      exit(IKS_ERROR_STRING_TO_X);
   }

   if(type2 == IKS_SIMBOLO_LITERAL_CHAR)
   {
      exit(IKS_ERROR_CHAR_TO_X);
   }

   if((type1 == IKS_SIMBOLO_LITERAL_STRING) || (type1 == IKS_SIMBOLO_LITERAL_CHAR))
   {
      exit(IKS_ERROR_WRONG_TYPE);
   }

   if(type1 == IKS_SIMBOLO_LITERAL_INT
      && (type2 == IKS_SIMBOLO_LITERAL_FLOAT || type2 == IKS_SIMBOLO_LITERAL_BOOL))
   {
      return IKS_SIMBOLO_LITERAL_INT;
   }

   if((type1 == IKS_SIMBOLO_LITERAL_FLOAT)
      && ((type2 == IKS_SIMBOLO_LITERAL_INT) || (type2 == IKS_SIMBOLO_LITERAL_BOOL)))
   {
      return IKS_SIMBOLO_LITERAL_FLOAT;
   }

   if((type1 == IKS_SIMBOLO_LITERAL_BOOL)
      && (type2 == IKS_SIMBOLO_LITERAL_FLOAT || type2 == IKS_SIMBOLO_LITERAL_INT))
   {
      return IKS_SIMBOLO_LITERAL_BOOL;
   }
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
	if(((type1 == IKS_SIMBOLO_LITERAL_INT) || (type1 == IKS_SIMBOLO_LITERAL_FLOAT) || (type1 == IKS_SIMBOLO_LITERAL_BOOL))
      && ((type2 == IKS_SIMBOLO_LITERAL_INT) || (type2 == IKS_SIMBOLO_LITERAL_FLOAT) || (type2 == IKS_SIMBOLO_LITERAL_BOOL)))
	{
      // Int
      if(type1 == IKS_SIMBOLO_LITERAL_INT && type2 == IKS_SIMBOLO_LITERAL_INT)
      {
         return IKS_SIMBOLO_LITERAL_INT;
      }

      if((type1 == IKS_SIMBOLO_LITERAL_INT && type2 == IKS_SIMBOLO_LITERAL_BOOL) 
         || (type1 == IKS_SIMBOLO_LITERAL_BOOL && type2 == IKS_SIMBOLO_LITERAL_INT))
      {
         return IKS_SIMBOLO_LITERAL_INT;
      }

      // Float
      if(type1 == IKS_SIMBOLO_LITERAL_FLOAT && type2 == IKS_SIMBOLO_LITERAL_FLOAT)
      {
         return IKS_SIMBOLO_LITERAL_FLOAT;
      }

      if((type1 == IKS_SIMBOLO_LITERAL_INT && type2 == IKS_SIMBOLO_LITERAL_FLOAT)
         || (type1 == IKS_SIMBOLO_LITERAL_FLOAT && type2 == IKS_SIMBOLO_LITERAL_INT))
      {
         return IKS_SIMBOLO_LITERAL_FLOAT;
      }

      if((type1 == IKS_SIMBOLO_LITERAL_BOOL && type2 == IKS_SIMBOLO_LITERAL_FLOAT)
         || (type1 == IKS_SIMBOLO_LITERAL_FLOAT && type2 == IKS_SIMBOLO_LITERAL_BOOL))
      {
         return IKS_SIMBOLO_LITERAL_FLOAT;
      }

      // Bool
      if(type1 == IKS_SIMBOLO_LITERAL_BOOL && type2 == IKS_SIMBOLO_LITERAL_BOOL)
      {
         return IKS_SIMBOLO_LITERAL_BOOL;
      }
   }
   else
   {
      if(((type1 == IKS_SIMBOLO_LITERAL_INT) || (type1 == IKS_SIMBOLO_LITERAL_FLOAT) || (type1 == IKS_SIMBOLO_LITERAL_BOOL))
         && ((type2 != IKS_SIMBOLO_LITERAL_INT) || (type2 != IKS_SIMBOLO_LITERAL_FLOAT) || (type2 != IKS_SIMBOLO_LITERAL_BOOL)))
      {
         if(type2 == IKS_SIMBOLO_LITERAL_CHAR)
         {
            exit(IKS_ERROR_CHAR_TO_X);
         }

         if(type2 == IKS_SIMBOLO_LITERAL_STRING)
         {
            exit(IKS_ERROR_STRING_TO_X);
         }
      }
      else if((type1 == IKS_SIMBOLO_LITERAL_STRING) || (type1 == IKS_SIMBOLO_LITERAL_CHAR))
      {
         exit(IKS_ERROR_WRONG_TYPE);
      }
   }
}


void sPop(STACK* pointer, comp_list_t* function, comp_list_t* local, int func_type)
{
   int flag = 0;	// 1- global var ----- 2- global vector ------- 3- local var
   comp_list_t* aux_list;
   comp_list_t *lastFunctionNameItem;
   comp_list_t *localList;
   STACK* aux_stack;

   int aux_type = pointer->disc->type;
   int aux_type1, aux_type2;

   if(pointer!=NULL)
   {
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
               printf("\nSYMBOL - TYPE: %d\n",pointer->disc->child->node_type);
               aux_type1 = pointer->disc->child->node_type;
               if(aux_type1 == 0) aux_type1 = pointer->disc->child->symbol->type;
            }

            if(pointer->disc->child->sibling != NULL)
            {
               printf("\nSIBLING IS NOT NULL");
               printf("\nSYMBOL - TOKEN: %s\n",pointer->disc->child->sibling->symbol->token);
               printf("\nSYMBOL - TYPE: %d\n",pointer->disc->child->sibling->node_type);
               aux_type2 = pointer->disc->child->sibling->node_type;
               if(aux_type2 == 0) aux_type2 = pointer->disc->child->sibling->symbol->type;
            }

            pointer->disc->node_type = inference(aux_type1,aux_type2);
            break;
	
         case IKS_AST_IDENTIFICADOR:
            variableName = pointer->disc;

            variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, variableName->symbol->token, declarationList);
            variableTypeLocal = getDeclarationDataType(IKS_LOCAL, variableName->symbol->token, declarationList);
            variableTypeVector = getDeclarationDataType(IKS_GLOBAL_VET, variableName->symbol->token, declarationList);
            variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, variableName->symbol->token, declarationList);

            // Checking if the variable was declared.
            if ((variableTypeGlobal == -1)
                && (variableTypeLocal == -1)
                && (variableTypeVector == -1)
                && (variableTypeFunction -1))
            {
               printf("Variavel '%s' nao declarada.\n", variableName->symbol->token);
               exit(IKS_ERROR_UNDECLARED);
            }
            break;
						
         case IKS_AST_LITERAL:
            pointer->disc->type = pointer->disc->symbol->type;
            pointer->disc->size = sizeDeclarations(pointer->disc->type);
            break;
						
         case IKS_AST_ATRIBUICAO:
            variableName = pointer->disc->child;
            data = pointer->disc->child->sibling;
            dataChild = data->child;

            // Checking if this is a variable declaration
            if(variableName->type == IKS_AST_IDENTIFICADOR)
            {
               variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, variableName->symbol->token, declarationList);
               variableTypeLocal = getDeclarationDataType(IKS_LOCAL, variableName->symbol->token, declarationList);
               variableTypeVector = getDeclarationDataType(IKS_GLOBAL_VET, variableName->symbol->token, declarationList);
               variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, variableName->symbol->token, declarationList);

               if(variableTypeVector != -1)
               {
                  printf("Variavel declarada como vetor.\n");
                  exit(IKS_ERROR_VECTOR);
               }
               else if(variableTypeFunction != -1)
               {
                  printf("Variavel declarada como funcao.\n");
                  exit(IKS_ERROR_FUNCTION);
               }
               else
               {
                  int variableType;
                  variableType = ((variableTypeGlobal != -1) ? variableTypeGlobal : variableTypeLocal);
                  pointer->disc->node_type = coertion(variableType, data->symbol->type);
               }
            }
            // Checking if this is a vector (global) declaration
            else if(variableName->type == IKS_AST_VETOR_INDEXADO)
            {
               variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, variableName->child->symbol->token, declarationList);
               variableTypeLocal = getDeclarationDataType(IKS_LOCAL, variableName->child->symbol->token, declarationList);
               variableTypeVector = getDeclarationDataType(IKS_GLOBAL_VET, variableName->child->symbol->token, declarationList);
               variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, variableName->child->symbol->token, declarationList);

               if(variableTypeGlobal != -1)
               {
                  printf("Vetor declarada como variável global.\n");
                  exit(IKS_ERROR_VARIABLE);
               }
               else if(variableTypeLocal != -1)
               {
                  printf("Vetor declarada como variável local.\n");
                  exit(IKS_ERROR_VARIABLE);
               }
               else if(variableTypeFunction != -1)
               {
                  printf("Vetor declarada como funcao.\n");
                  exit(IKS_ERROR_FUNCTION);
               }
               else
               {
                  pointer->disc->node_type = coertion(variableTypeVector, data->symbol->type);
               }
            }
            break;

         case IKS_AST_INPUT:
            if(pointer->disc->child->type != IKS_AST_IDENTIFICADOR)
            {
               printf("O parametro do comando INPUT nao é um identificador\n");
               exit(IKS_ERROR_WRONG_PAR_INPUT);
            }
            else
            {
               pointer->disc->node_type = pointer->disc->child->node_type;
            }
            break;

         case IKS_AST_OUTPUT:
            if(pointer->disc->child->type != IKS_SIMBOLO_LITERAL_CHAR)
            {
               printf("O parametro do comando OUTPUT deve ser literal do tipo string\n");
               exit(IKS_ERROR_WRONG_PAR_OUTPUT);
            }
            else
            {
               pointer->disc->node_type = pointer->disc->child->node_type;
            }
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

      if(pointer != NULL)
      {
         return sPop(pointer, listFunctions, declarationList, 0);
      }
   }
   else
   {
      printf("\nPOINTER NULL");
   }
}
