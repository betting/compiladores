/**
 * @file semantic.c
 * @version 1.0
 *
 * @section DESCRIPTION
 * Semantic Analisys
 * Perform a semantic analisys in a code provided by the user
 */

#include <stdlib.h>
#include <string.h>
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
int variableTypeFuncParamLocal;

char *actualFunctionName;

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
      case IKS_FUNC_PARAM:
         lastFunctionNameItem = getLastItemList(listFunctions);
         localList = getLocalList(declarationList, lastFunctionNameItem);
        
         if(searchToken(localList, dictNode->token) == NULL)
         {
            if (escopo == IKS_LOCAL)
            {
               declarationList = addItem(dictNode->type, dictNode->token, declarationList, IKS_LOCAL);
            }
            else
            {
               declarationList = addItem(dictNode->type, dictNode->token, declarationList, IKS_FUNC_PARAM);
            }
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

/**
 * Validate all operations recursively
 *
 * @param   *operationNode       The first operation element found in the stack
 * @return                       The data type returned by inference function
 */
int validateOperation(comp_tree_t* operationNode)
{
   if (operationNode->symbol != NULL)
   {
      if (operationNode->sibling != NULL)
      {
         return inference(operationNode->symbol->type, operationNode->sibling->symbol->type);
      }
      // Function calls without any parameter
      else
      {
         return inference(operationNode->symbol->type, operationNode->symbol->type);
      }
   }
   else
   {
      int nodeType;
      nodeType = validateOperation(operationNode->child);
      return inference(nodeType, operationNode->sibling->symbol->type);
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
            pointer->disc->node_type = validateOperation(pointer->disc->child);
            break;
	
         case IKS_AST_IDENTIFICADOR:
            variableName = pointer->disc;

            variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, variableName->symbol->token, declarationList, NULL);
            variableTypeLocal = getDeclarationDataType(IKS_LOCAL, variableName->symbol->token, declarationList, lastFunction->symbol->token);
            variableTypeFuncParamLocal = getDeclarationDataType(IKS_FUNC_PARAM, variableName->symbol->token, declarationList, lastFunction->symbol->token);
            variableTypeVector = getDeclarationDataType(IKS_GLOBAL_VET, variableName->symbol->token, declarationList, NULL);
            variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, variableName->symbol->token, declarationList, NULL);

            // Checking if the variable was declared.
            if ((variableTypeGlobal == -1)
                && (variableTypeLocal == -1)
                && (variableTypeFuncParamLocal == -1)
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
               variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, variableName->symbol->token, declarationList, NULL);
               variableTypeLocal = getDeclarationDataType(IKS_LOCAL, variableName->symbol->token, declarationList, lastFunction->symbol->token);
               variableTypeFuncParamLocal = getDeclarationDataType(IKS_FUNC_PARAM, variableName->symbol->token, declarationList, lastFunction->symbol->token);
               variableTypeVector = getDeclarationDataType(IKS_GLOBAL_VET, variableName->symbol->token, declarationList, NULL);
               variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, variableName->symbol->token, declarationList, NULL);

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
                  variableType = ((variableType != -1) ? variableType : variableTypeFuncParamLocal);

                  int dataType;
                  // If this is an operation, the values will checked before perform coertion.
                  if ((data->type != IKS_AST_LITERAL) || (data->type != IKS_AST_IDENTIFICADOR))
                  {
                     dataType = validateOperation(data->child);
                  }
                  // Simple value attribution
                  else
                  {
                     dataType = data->symbol->type;
                  }
                  pointer->disc->node_type = coertion(variableType, dataType);
               }
            }
            // Checking if this is a vector (global) declaration
            else if(variableName->type == IKS_AST_VETOR_INDEXADO)
            {
               variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, variableName->child->symbol->token, declarationList, NULL);
               variableTypeLocal = getDeclarationDataType(IKS_LOCAL, variableName->child->symbol->token, declarationList, lastFunction->symbol->token);
               variableTypeFuncParamLocal = getDeclarationDataType(IKS_FUNC_PARAM, variableName->child->symbol->token, declarationList, lastFunction->symbol->token);
               variableTypeVector = getDeclarationDataType(IKS_GLOBAL_VET, variableName->child->symbol->token, declarationList, NULL);
               variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, variableName->child->symbol->token, declarationList, NULL);

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
               else if(variableTypeFuncParamLocal != -1)
               {
                  printf("Vetor declarada como parametro de funcao.\n");
                  exit(IKS_ERROR_VARIABLE);
               }
               else if(variableTypeFunction != -1)
               {
                  printf("Vetor declarada como funcao.\n");
                  exit(IKS_ERROR_FUNCTION);
               }
               else
               {

                  int dataType;
                  // If this is an operation, the values will checked before perform coertion.
                  if ((data->type != IKS_AST_LITERAL) || (data->type != IKS_AST_IDENTIFICADOR))
                  {
                     dataType = validateOperation(data->child);
                  }
                  // Simple value attribution
                  else
                  {
                     dataType = data->symbol->type;
                  }
                  pointer->disc->node_type = coertion(variableTypeVector, dataType);
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
            if(pointer->disc->child->type == IKS_AST_CHAMADA_DE_FUNCAO)
            {
               variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, pointer->disc->child->child->symbol->token, declarationList, NULL);
               pointer->disc->node_type = coertion(lastFunction->node_type, variableTypeFunction);
            }
            else if(lastFunction->node_type != pointer->disc->child->symbol->type)
            {
               if((pointer->disc->child->symbol->type == IKS_SIMBOLO_LITERAL_CHAR)
                  ||(pointer->disc->child->symbol->type == IKS_SIMBOLO_LITERAL_STRING))
               {
                     printf("Tipo do comando return e tipo da funcao diferentes\n");
                     exit(IKS_ERROR_WRONG_PAR_RETURN);
               }
               else
               {
                  int dataType;
                  // If this is an operation, the values will checked before perform coertion.
                  if ((pointer->disc->type != IKS_AST_LITERAL) || (pointer->disc->type != IKS_AST_IDENTIFICADOR))
                  {
                     dataType = validateOperation(pointer->disc->child);
                  }
                  // Simple value attribution
                  else
                  {
                     dataType = pointer->disc->symbol->type;
                  }
                  pointer->disc->node_type = coertion(lastFunction->node_type, dataType);
               }
            }
            break;

         case IKS_AST_FUNCAO:
            lastFunction = pointer->disc;
			
            variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, lastFunction->symbol->token, declarationList, NULL);
            lastFunction->node_type = variableTypeFunction;
            break;

         case IKS_AST_CHAMADA_DE_FUNCAO:
            actualFunctionName = (char*)calloc(strlen(pointer->disc->child->symbol->token)+1,sizeof(char));
            strcpy(actualFunctionName, pointer->disc->child->symbol->token);

            variableTypeFunction = getDeclarationDataType(IKS_FUNCTION, actualFunctionName, declarationList, NULL);
            if (variableTypeFunction != -1)
            {
               comp_list_t* paramList;
               paramList = getLocalDeclarations(actualFunctionName, declarationList, IKS_FUNC_PARAM);
               
               int numParam = countParam(paramList, pointer->disc->child->sibling);
               if (numParam < 0)
               {
                  printf("Numero de paremetros excedido em '%s'.\n", actualFunctionName);
                  exit(IKS_ERROR_EXCESS_ARGS);
               }
               else if (numParam > 0)
               {
                  printf("Faltam argumentos na chamada da funcao '%s'.\n", actualFunctionName);
                  exit(IKS_ERROR_MISSING_ARGS);
               }

            }
            else
            {
               printf("Funcao '%s' nao declarada.\n", actualFunctionName);
               exit(IKS_ERROR_UNDECLARED);
            }


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

/**
 * Calculate the difference between parameter list and parameters found in the function call.
 *
 * @param   *paramList           List with all parameters declared
 * @param   *paramFoundInCall    All parameters added in the function call
 * @return                       If result < 0 then the number of parameters used in the function call exceed the expected.
 *                               If result == 0 then both lists have the number of parameters.
 *                               Finally, if result > 0 then it was declared more parameters than the provided in the function call.
 */
int countParam(comp_list_t* paramList, comp_tree_t* paramFoundInCall)
{
   int countParamList = 0;

   while (paramList != NULL)
   {
      countParamList++;
      paramList = paramList->next;
   }

   int countParamFoundInCall = 0;
   while (paramFoundInCall != NULL)
   {
      countParamFoundInCall++;
      paramFoundInCall = paramFoundInCall->child;
   }

   return (countParamList - countParamFoundInCall);
}
