#include <stdlib.h>
#include <string.h>
#include "tac.h"

int varType;
int variableTypeGlobal;
int variableTypeLocal;

TAC* CodeGenerate(comp_tree_t* nodo,TAC* code, int iloc_code, comp_list_t* declarations, char *actualFunction)
{
   
   TAC* aux_tac;
   TAC* jump;
   comp_tree_t* currentNodo;
	
   switch(iloc_code)
   {
      
      case ILOC_NOP:
         nodo->code = initTac();
         nodo->code->code = ILOC_NOP;

         //printf("\nnop: %d\n", ILOC_NOP);
//         return code;
         return nodo->code;

         break;

      case ILOC_ADD:
         code = Operator2(nodo, ILOC_ADD);
         code = insertTAC(nodo);

         //printLabel(code);
         //printCode(code);

//         printf("add r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_SUB:
         code = Operator2(nodo, ILOC_SUB);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("sub r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_MULT:
         code = Operator2(nodo, ILOC_MULT);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("mult r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_DIV:
         code = Operator2(nodo, ILOC_DIV);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("div r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_ADDI:
         break;

      case ILOC_SUBI:
         break;

      case ILOC_RSUBI:
         break;

      case ILOC_MULTI:
         break;

      case ILOC_DIVI:
         break;

      case ILOC_RDIVI:
         break;

      case ILOC_LSHIFT:
         break;

      case ILOC_LSHIFTI:
         break;

      case ILOC_RSHIFT:
         break;

      case ILOC_RSHIFTI:
         break;

      case ILOC_AND:
         code = Operator2(nodo, ILOC_AND);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("and r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_ANDI:
         break;

      case ILOC_OR:
         code = Operator2(nodo, ILOC_OR);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("or r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;


      case ILOC_LOADI:
         nodo->code = initTac();
         reg = getLabelReg(reg);
         nodo->code->r3 = reg;
         nodo->code->constant = atoi(nodo->symbol->token);
         nodo->code->code = ILOC_LOADI;
//         code = insertTAC(nodo);

//         printLabel(code);
//         printf("loadI %d => r%d\n",code->constant,code->r3);
         return code;
         break;

      case ILOC_LOAD:
         break;

      case ILOC_LOADAI:
         // Handling common variable (Global and Local)
         if (nodo->type == IKS_AST_IDENTIFICADOR)
         {
            variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, nodo->symbol->token, declarations, NULL);
            if (variableTypeGlobal == -1)
            {
               variableTypeLocal = getDeclarationDataType(IKS_LOCAL, nodo->symbol->token, declarations, actualFunction);
               varType = FP;
            }
            else
            {
               varType = BSS;
            }

            nodo->code = initTac();
            reg = getLabelReg(reg);
            nodo->code->r1 = varType;
            nodo->code->r3 = reg;
            nodo->code->constant = -1;
            nodo->code->code = ILOC_LOADAI;
         }
         // Handling a vector (Global only)
         else if (nodo->type == IKS_AST_VETOR_INDEXADO)
         {
//            aux_tac = CodeGenerate(nodo, code, ILOC_NOP, NULL, NULL);
//            aux_tac = aux_tac->next;
//            code = Address(node);         
            
         }

//         printLabel(code);
//         printf("loadAI %s, %d => r%d\n", (varType == IKS_GLOBAL_VAR)?"global":"local", code->constant, code->r3);
         return code;
         break;


      case ILOC_STORE:
         nodo->code = initTac();

         // Simple attribution
         if (nodo->child[1]->code != NULL)
         {
            nodo->code->r1 = nodo->child[1]->code->r3;

            if (nodo->child[1]->code->next != NULL)
            {
               nodo->code->r1 = nodo->child[1]->code->next->r3;
            }
         }
         // Getting value from a function return
         else
         {
            nodo->code->r1 = nodo->child[1]->child[0]->code->next->r3;
         }
         nodo->code->r3 = nodo->child[0]->code->r3;
         nodo->code->code = ILOC_STORE;
         if (nodo->child[2] == NULL)
         {
               code = insertTAC(nodo);
         }
         else
         {
            code = insertTacEvaluated(nodo, code);
         }

//         printLabel(code);
//         printf("store r%d => r%d\n",code->r1,code->r3);
         return code;
         break;

      case ILOC_JUMPI:
			nodo->code = initTac();
			nodo->code->l1 = label;
			nodo->code->code = ILOC_JUMPI;
			code = insertTAC(nodo);
			return code;
         break;


      case ILOC_CBR:
         // Getting test result (child[0])
         nodo->code = initTac();
         nodo->code->r3 = nodo->child[0]->code->r3;

         // Manipulating the first block (Then)
         InsertLabel(nodo->child[1]);

//         code = insertTAC(nodo->child[1]);
         // Adding code in the sibling
         if(nodo->child[2] == NULL)
         {
            nodo->child[2] = createNewNode(0);
            code = CodeGenerate(nodo->child[2], code, ILOC_NOP, NULL, NULL);
         }
			InsertLabel(nodo->child[2]);

         // Updating parent node with children nodes.
			nodo->code->l1 = label-1;
			nodo->code->l2 = label; 
			nodo->code->next = NULL;
			nodo->code->code = ILOC_CBR;

//			printf("cbr r%d => l%d, l%d\n", nodo->code->r3, nodo->code->l1, nodo->code->l2);

			//code = concatTAC(code,nodo->code);
			if(nodo->child[3] == NULL)
			{
					code = combineCTE(nodo,CTE_IF);
			}
			else
			{
					InsertLabel(nodo->child[3]);
					code = CodeGenerate(nodo->child[1], code, ILOC_JUMPI, NULL, NULL);
					code = combineCTE(nodo,CTE_IF_ELSE);
			}

//			printf("cbr r%d => l%d, l%d\n", nodo->code->r3, nodo->code->l1, nodo->code->l2);
			
			
			return code;
			
         break;
/*
      case ILOC_CBR_WHILE:
			jump = initTac();
			jump->code = ILOC_JUMPI;
			nodo->code = initTac();
			nodo->code->r3 = nodo->child->code->r3;
			if(nodo->child->sibling->sibling == NULL)
			{
				nodo->child->sibling->sibling = (comp_tree_t*)malloc(sizeof(comp_tree_t));
//				nodo->child->sibling->child = (comp_tree_t*)malloc(sizeof(comp_tree_t));
				code = CodeGenerate(nodo->child->sibling->sibling, code, ILOC_NOP, NULL, NULL);
			}
			label = getLabelReg(label);
			nodo->child->code->label = label;
			jump->l1 = label;
			InsertLabel(nodo->child->sibling);
			InsertLabel(nodo->child->sibling->sibling);
			nodo->code->l1 = label - 1; 
			nodo->code->l2 = label; 
			nodo->code->next = NULL;
			nodo->code->code = ILOC_CBR;
			aux_tac = nodo->child->sibling->code;
			nodo->child->sibling->code = jump;
			jump->next = aux_tac;
			nodo->code = combineCTE(nodo, CTE_IF);
			return nodo->code;
		break;   

      case ILOC_CBR_DO:
			InsertLabel(nodo->child);
			jump->code = ILOC_JUMPI;
			jump->l1 = label;
			nodo->code = initTac(); 
			nodo->code->l2 = label; 
			nodo->code->code = ILOC_CBR;
			if(nodo->child->type == IKS_AST_BLOCO)
				nodo->code->r3 = nodo->child->sibling->code->r3;
			else
				nodo->code->r3 = nodo->child->code->r3;
			nodo->code->l1 = label;
			if(nodo->child->sibling->sibling == NULL)
			{
				nodo->child->sibling->sibling = (comp_tree_t*)malloc(sizeof(comp_tree_t));
				code = CodeGenerate(nodo->child->sibling->sibling, code, ILOC_NOP, NULL, NULL);
			}
			InsertLabel(nodo->child->sibling->sibling);
			nodo->code->l2 = label; 
			aux_tac = nodo->code;
			nodo->code = jump;
			nodo->code->next = aux_tac;
			nodo->code = insertTAC(nodo);
			return nodo->code;
        break; 
*/
      case ILOC_CMP_LT:
         code = Operator2(nodo, ILOC_CMP_LT);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("cmp_lt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_LE:
         code = Operator2(nodo, ILOC_CMP_LE);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("cmp_le r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_EQ:
         code = Operator2(nodo, ILOC_CMP_EQ);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("cmp_eq r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_GE:
         code = Operator2(nodo, ILOC_CMP_GE);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("cmp_ge r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_GT:
         code = Operator2(nodo, ILOC_CMP_GT);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("cmp_gt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_NE:
         code = Operator2(nodo, ILOC_CMP_NE);
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("cmp_ne r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;
   }
   
}


TAC* Operator2(comp_tree_t* nodo, int operatorCode)
{ 
   nodo->code = initTac();
   nodo->code->r1 = nodo->child[0]->code->r3;
   nodo->code->r2 = nodo->child[1]->code->r3;
   nodo->code->code = operatorCode;
   reg = getLabelReg(reg);
   nodo->code->r3 = reg;
   
   return nodo->code;
   
}

TAC* initTac()
{
   TAC* new =(TAC*)malloc(sizeof(TAC));
   new->r1 = 0;
   new->r2 = 0;
   new->r3 = 0;
   new->l1 = 0;
   new->l2 = 0;
   new->label = 0;
   new->constant = 0;
   new->code = 0;
   new->next = NULL;
   return new;
}


TAC* insertTAC(comp_tree_t* nodo)
{
   TAC * aux = nodo->code;

   if (nodo->child[3] != NULL)
   {
      aux = concatTAC(aux, nodo->child[3]->code);
   }

   if (nodo->child[2] != NULL)
   {
      aux = concatTAC(aux, nodo->child[2]->code);
   }

   if (nodo->child[1] != NULL)
   {
      aux = concatTAC(aux, nodo->child[1]->code);
   }

   if (nodo->child[0] != NULL)
   {
      aux = concatTAC(aux, nodo->child[0]->code);
   }

   return nodo->code;
   
}


TAC* concatTAC(TAC* parent, TAC* child)
{
   if ((parent != NULL) && (child != NULL))
   {
      TAC * aux = parent;
      while(aux->next != NULL)
      {
   	   aux = aux->next;
      }
      aux->next = child;
   }
   return parent;
}

TAC* insertTacEvaluated(comp_tree_t* nodo, TAC* code)
{
   nodo->code = concatTAC(nodo->code, nodo->child[1]->code);
   nodo->code = concatTAC(nodo->code, nodo->child[0]->code);

   TAC* aux = nodo->code;
   if (nodo->child[2] != NULL)
   {
      nodo->code = nodo->child[2]->code;
      nodo->code = concatTAC(nodo->code, aux); 
   }
   return nodo->code;
}

comp_tree_t* getLastSibling(comp_tree_t* nodo)
{
}

int countSiblings(comp_tree_t* nodo)
{
}

void InsertLabel(comp_tree_t* nodo)
{
   TAC* aux;
   aux = nodo->code;
   
   while(aux->next != NULL)
   {
      aux = aux->next;
   }
   label = getLabelReg(label);
   aux->label = label;
}


void printLabel(TAC* code)
{
   if (code->labelName != NULL)
   {
      printf("%s:\n", code->labelName);
   }
   else if(code->label != 0) 
   {
      printf("l%d:\n",code->label);
   }
}

TAC* combineCTE(comp_tree_t* nodo, int caseCTE)
{
	TAC* aux;
	comp_tree_t* aux_nodo;
	aux_nodo = nodo;
	aux = nodo->code;
	
	switch(caseCTE)
	{	//if simples
		case CTE_IF:
            if(aux_nodo->child[2] != NULL)
            {
               nodo->code = aux_nodo->child[2]->code;
            }

				concatTAC(nodo->code,aux_nodo->child[1]->code);
				concatTAC(nodo->code,aux);
				concatTAC(nodo->code,aux_nodo->child[0]->code);
				return nodo->code;
			break;
		//if-else
		case CTE_IF_ELSE:
				nodo->code = aux_nodo->child[3]->code;
				concatTAC(nodo->code,aux_nodo->child[2]->code);
				concatTAC(nodo->code,aux_nodo->child[1]->code);

				concatTAC(nodo->code,aux);
				concatTAC(nodo->code,aux_nodo->child[0]->code);
				return nodo->code;
			break;
		//while
		case CTE_WHILE:
				return nodo->code;
			break;
	}
}

void printCode(TAC* code)
{
   
   printf("\nr1: %d", code->r1);
   printf("\nr2: %d", code->r2);
   printf("\nr3: %d", code->r3);
   printf("\nl1: %d", code->l1);
   printf("\nl2: %d", code->l2);
   printf("\nlabel: %d", code->label);
   printf("\nconstant: %d", code->constant);
   printf("\ncode: %d", code->code);
   if(code->next==NULL)
   {   printf("\nNEXT: NULL"); }
   else
	{	printf("\nNEXT: NOT NULL"); }

}

void printAssembly(TAC* code)
{
   char* valorStr1= (char *)malloc(sizeof(char));
   char* valorStr2= (char *)malloc(sizeof(char));
   while(code != NULL)
   {
	  //printCode(code);
      switch(code->code)
      {
         case ILOC_NOP:
            printLabel(code);
            printf("\tnop\n");
            break;
         case ILOC_ADD:
            printLabel(code);
            printf("\tadd r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_SUB:
            printLabel(code);
            printf("\tsub r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_MULT:
            printLabel(code);
            printf("\tmult r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_DIV:
            printLabel(code);
            printf("\tdiv r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_ADDI:
            printLabel(code);
            printf("\taddi %s, %d => %s\n", getRegisterName(code->r1), code->constant, getRegisterName(code->r3));
            break;
         case ILOC_SUBI:
            printLabel(code);
            printf("\tsubi %s, %d => %s\n", getRegisterName(code->r1), code->constant, getRegisterName(code->r3));
            break;
         case ILOC_RSUBI:
            printLabel(code);
            break;
         case ILOC_MULTI:
            printLabel(code);
            break;
         case ILOC_DIVI:
            printLabel(code);
            break;
         case ILOC_RDIVI:
            printLabel(code);
            break;
         case ILOC_LSHIFT:
            printLabel(code);
            break;
         case ILOC_LSHIFTI:
            printLabel(code);
            break;
         case ILOC_RSHIFT:
            printLabel(code);
            break;
         case ILOC_RSHIFTI:
            printLabel(code);
            break;
         case ILOC_AND:
            printLabel(code);
            printf("\tand r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_ANDI:
            printLabel(code);
            break;
         case ILOC_OR:
            printLabel(code);
            printf("\tor r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_ORI:
            printLabel(code);
            break;
         case ILOC_XOR:
            printLabel(code);
            break;
         case ILOC_XORI:
            printLabel(code);
            break;
         case ILOC_LOADI:
            printLabel(code);
            printf("\tloadi %d => %s\n", code->constant, getRegisterName(code->r3));
            break;
         case ILOC_LOAD:
            printLabel(code);
            printf("\tload %s => %s\n", getRegisterName(code->r1), getRegisterName(code->r2));
            break;
         case ILOC_LOADAI:
            printLabel(code);
            printf("\tloadai %s, %d => %s\n", getRegisterName(code->r1), code->constant, getRegisterName(code->r3));
            break;
         case ILOC_LOADAO:
            printLabel(code);
            break;
         case ILOC_CLOAD:
            printLabel(code);
            break;
         case ILOC_CLOADAI:
            printLabel(code);
            break;
         case ILOC_CLOADAO:
            printLabel(code);
            break;
         case ILOC_STORE:
            printLabel(code);
            printf("\tstore %s => %s\n",getRegisterName(code->r1),getRegisterName(code->r3));
            break;
         case ILOC_STOREAI:
            printLabel(code);
            printf("\tstoreai %s => %s, %d\n", getRegisterName(code->r1), getRegisterName(code->r2), code->constant);
            break;
         case ILOC_STOREAO:
            printLabel(code);
            break;
         case ILOC_CSTORE:
            printLabel(code);
            break;
         case ILOC_CSTOREAI:
            printLabel(code);
            break;
         case ILOC_CSTOREAO:
            printLabel(code);
            break;
         case ILOC_I2I:
            printf("\ti2i %s => %s\n", getRegisterName(code->r1), getRegisterName(code->r2));
            printLabel(code);
            break;
         case ILOC_C2C:
            printLabel(code);
            break;
         case ILOC_C2I:
            printLabel(code);
            break;
         case ILOC_I2C:
            printLabel(code);
            break;
         case ILOC_JUMPI:
            if (code->constant != 99999)
            {
//               printLabel(code);
            }
            if (code->labelName != NULL)
            {
   			   printf("\tjumpI => %s\n", code->labelName);
            }
            else
            {
      			printf("\tjumpI => l%d\n", code->label);
            }
            break;
         case ILOC_JUMP:
            printf("\tjump => %s\n", getRegisterName(code->r1));
//            printf("\tjump => %s\n", code->labelName);
            break;
         case ILOC_CBR:
            printLabel(code);
   			printf("\tcbr r%d => l%d, l%d\n", code->r3, code->l1, code->l2);
            break;
         case ILOC_CMP_LT:
            printLabel(code);
            printf("\tcmp_lt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_LE:
            printLabel(code);
            printf("\tcmp_le r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_EQ:
            printLabel(code);
            printf("\tcmp_eq r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_GE:
            printLabel(code);
            printf("\tcmp_ge r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_GT:
            printLabel(code);
            printf("\tcmp_gt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_NE:
            printLabel(code);
            printf("\tcmp_ne r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_FUNCTION:
            printLabel(code);
            break;
         case ILOC_HALT:
            printf("end:\n");
            printf("\thalt\n");
            break;
      }
      code = code->next;
   }
}

TAC* invertTacList(TAC* list)
{
   TAC *current;
   TAC *rest;
   if(list == NULL)
   {
      return list;
   }

   current = list;
   rest = list->next;

   if(rest == NULL)
   {
      return current;
   }

   rest = invertTacList(rest);

   current->next->next = current;
   current->next = NULL;

   return rest;
}

TAC* Address(comp_tree_t* nodo)
{
/*
 *	1 - testar se o nodo é nulo
 *    - não sendo nulo, verifica se é um nop
 * 		- 	se for nop vai incrementando o valor
 *  2 - criar cópia da árvore
 * 		- remover os nops existente na cópia da árvore
 * 	3 - se o incremento > 0
 * 		- fazer o load dos registradores
 * 		- concatenar o nodo
 *  4 - tá faltando alguma coisa?
 */
	 //pegar valores e colocar numa lista
	 comp_list_t* lista_auxiliar, list;
	 //lista_auxiliar = searchToken(,nodo->child[0]->symbol->text);
	 int count = 0;
	 comp_tree_t* aux;
	 aux = nodo;
	 if(lista_auxiliar!=NULL)
	 {
	 }

	//tratar o resultado e devolver pro nodo
	return nodo->code;

}

TAC* CodeGenerateFuncDeclaration(comp_tree_t* nodo, TAC* code, comp_list_t* declarations)
{
   nodo->code = CodeGenerate(nodo, code, ILOC_NOP, NULL, NULL);
   nodo->code->code = ILOC_FUNCTION;

   if (nodo->symbol != NULL)
   {
      nodo->code->labelName = (char*)calloc(strlen(nodo->symbol->token)+1,sizeof(char));
      strcpy(nodo->code->labelName, nodo->symbol->token);
   }

   TAC* aux;
   RA* frame;
   int frameSize;
   // Stack configuration for "main" function
   if (strcmp(nodo->code->labelName, "main") == 0)
   {
      frame = calculateFrameSize("main", nodo, declarations);
      frameSize = frame->localVarSize + frame->paramSize + frame->returnSize + frame->staticLinkSize + frame->dynamicLinkSize;
      aux = initTac();
      //aux->constant = localContextSize;
      aux->constant = frameSize;
      aux->r3 = SP;
      aux->code = ILOC_LOADI;

      aux->next = initTac();
      aux->next->constant = 0;
      aux->next->r3 = FP;
      aux->next->code = ILOC_LOADI;
      aux->next->label = 0;

      aux->next->next = nodo->code;

      nodo->code = aux;

   }
   // Initial stack allocation for other functions
   else
   {
      frame = calculateFrameSize(nodo->code->labelName, nodo, declarations);
      frameSize = frame->localVarSize + frame->paramSize + frame->returnSize + frame->staticLinkSize + frame->dynamicLinkSize;

      int returnPos = frame->localVarSize + frame->paramSize;
      int retValue = frame->localVarSize + frame->paramSize;

      TAC* returnTac = nodo->child[0]->code;
      int tacFound = FALSE;
      while ((tacFound != TRUE) || (returnTac != NULL))
      {
         if (returnTac->constant == -1)
         {  
            returnPos = (returnPos > 0) ? returnPos - 4 : 0;
            retValue = retValue - 4;
            returnTac->constant = returnPos;
            tacFound = TRUE;
         }
         returnTac = returnTac->next;
      }
   }

   if (code != NULL)
   {
      TAC* aux_tmp = code;
      aux_tmp = concatTAC(aux_tmp, nodo->code);
      nodo->code = aux_tmp;
   }

   return nodo->code;
}

TAC* CodeGenerateFuncCall(comp_tree_t* nodo, TAC* code, comp_list_t* declarations)
{

/*
   1. Cria uma novo registro de ativação
   2. Calcula o vinculo estático
   3. Passa os parâmetros (organizando-os na pilha)
   4. Passa o endereço de retorno para o chamado
   5. Transfere controle para o chamado
   6. Salva o estado de execução atual (registradores)
   7. Salva o antigo FP na pilha (como vínculo dinâmico)
   8. Aloca variáveis locais
*/

   TAC* finalTac;
   int fp = 0;

   RA* arData = calculateFrameSize(nodo->child[0]->symbol->token, nodo, declarations);

   int frameSize = arData->localVarSize + arData->paramSize + arData->returnSize + arData->staticLinkSize + arData->dynamicLinkSize;
   
   TAC* newCode;
   // Allocating space for the stack frame
   newCode = initTac();
   newCode->r1 = SP;

   reg = getLabelReg(reg);
   newCode->r2 = reg;
   newCode->code = ILOC_LOAD;
   finalTac = newCode;

   newCode = initTac();
   newCode->r1 = reg;
   newCode->constant = frameSize;
   newCode->r3 = SP;
   newCode->code = ILOC_ADDI;
   finalTac = concatTAC(finalTac, newCode);

   // If the function called expects parametersi, then
   // they will be pushed in the stack.
   if (arData->paramSize > 0)
   {
      int i;
      for (i = 0; i < arData->paramQuantity; i++)
      {
         newCode = initTac();
         newCode->r1 = -1;
         newCode->r2 = FP;
         newCode->constant = (i * 4) + 4;
         newCode->code = ILOC_STOREAI;
         finalTac = concatTAC(finalTac, newCode);
         fp = fp + newCode->constant;
      }
   } 

   // Allocating static link space in the stack
   fp = fp + arData->staticLinkSize;

   // Allocating dynamic link space in the stack
   fp = fp + arData->dynamicLinkSize;
         
   // Save the return address in the stack (after JMP)
   newCode = initTac();
   newCode->r1 = FP;
   newCode->r3 = SP;
   newCode->code = ILOC_STORE;
   finalTac = concatTAC(finalTac, newCode);

   // Updating SP
   newCode = initTac();
   newCode->r1 = reg;
   newCode->r2 = FP;
   newCode->code = ILOC_I2I;
   finalTac = concatTAC(finalTac, newCode);

   // Calling the function
   newCode = initTac();
   newCode->labelName = nodo->child[0]->symbol->token;
   newCode->code = ILOC_JUMPI;
   finalTac = concatTAC(finalTac, newCode);

   // Loading the return content
   newCode = initTac();
   newCode->r1 = SP;
   newCode->constant = -8;
   reg = getLabelReg(reg);
   newCode->r3 = reg;
   newCode->code = ILOC_LOADAI;
   finalTac = concatTAC(finalTac, newCode);
   
   // Updating back the SP
   newCode = initTac();
   newCode->r1 = SP;
   newCode->constant = -frameSize;
   newCode->r3 = SP;
   newCode->code = ILOC_LOADAI;
   finalTac = concatTAC(finalTac, newCode);

   // Reordering the list.
   finalTac = invertTacList(finalTac);
   nodo->code = finalTac;
   
   return nodo->code; 
}

TAC* CodeGenerateReturn(comp_tree_t* nodo, TAC* code, comp_list_t* declarations)
{
/*
   1. Prepara os parâmetros de retorno
   2. Disponibiliza o valor de retorno para o chamador
   3. Atualiza o FP e o SP
   4. Atualiza o estado de execução do chamador
   5. Transfere o controle
*/
   
   TAC* finalTac;
   int fp = 0;

   RA* arData = calculateFrameSize(nodo->child[0]->symbol->token, nodo, declarations);

   int frameSize = arData->localVarSize + arData->paramSize + arData->returnSize + arData->staticLinkSize + arData->dynamicLinkSize;

   TAC* newCode;

   // Getting return (number)
   if (nodo->child[0]->symbol->type != IKS_SIMBOLO_IDENTIFICADOR)
   {
      reg = getLabelReg(reg);
      newCode = initTac();
      newCode->constant = atoi(nodo->child[0]->symbol->token);
      newCode->r3 = reg;
      newCode->code = ILOC_LOADI;
      newCode->label = 0;
      finalTac = newCode;

      // Save the return address in the stack
      newCode = initTac();
      newCode->r1 = reg;
      newCode->r2 = FP;
      newCode->constant = -1;
      newCode->code = ILOC_STOREAI;
      finalTac = concatTAC(finalTac, newCode);
   }
   // Getting retun value from a variable
   else
   {
      // Save the return address in the stack
      newCode = initTac();
      newCode->r1 = reg-2;
      newCode->r2 = FP;
      newCode->constant = arData->localVarSize + arData->paramSize + arData->returnSize;
      newCode->code = ILOC_STOREAI;
      finalTac = newCode;
   }

   // 3. Atualizando FP
   newCode = initTac();
   newCode->code = ILOC_LOADAI;
   newCode->r1 = FP;
   newCode->constant = arData->localVarSize + arData->paramSize + arData->returnSize;
   newCode->r3 = FP;
   finalTac = concatTAC(finalTac, newCode);

   newCode = initTac();
   newCode->code = ILOC_ADDI;
   newCode->r1 = FP;
   newCode->constant = 4;

   reg = getLabelReg(reg);
   newCode->r3 = reg;
   finalTac = concatTAC(finalTac, newCode);

   // Jumping back to the caller function
   newCode = initTac();
   newCode->r1 = reg;
   newCode->code = ILOC_JUMP;
   finalTac = concatTAC(finalTac, newCode);

   // Reordering the list
   finalTac = invertTacList(finalTac);
   nodo->code = finalTac;
   return nodo->code;
}

TAC* initCode(TAC* code, comp_tree_t* nodo, comp_list_t * declarations)
{
   // Getting the first function declared in the AST
   comp_tree_t* programa = nodo->child[0];


   // Jumping to main function
   TAC *assembly = initTac();
   assembly->code = ILOC_JUMPI;
   assembly->constant = 99999;
   assembly->labelName = (char*)malloc(5*sizeof(char));
   strcpy(assembly->labelName, "main");

   // Navigating thru the functions
   int mainFound = FALSE;
   while (programa != NULL)
   {
      if (programa->child[0] != NULL)
      {
         TAC *aux_code = (programa->child[0]->code != NULL) ? programa->child[0]->code : programa->child[0]->child[0]->code;

         aux_code = invertTacList(aux_code);
         if (aux_code->labelName != NULL)
         {
            mainFound = (strcmp(aux_code->labelName, "main") == 0);
         }
         else
         {
            mainFound = FALSE;
         }

         aux_code = evaluateFinalTac(aux_code, declarations);

         assembly = concatTAC(assembly, aux_code);
         
         if (mainFound == TRUE)
         {
            TAC *jmp_end = initTac();
            jmp_end->code = ILOC_JUMPI;
            jmp_end->labelName = (char *)malloc(sizeof(char));
            strcpy(jmp_end->labelName, "end");
            assembly = concatTAC(assembly, jmp_end);
         }
      }
      else if (programa->code != NULL)
      {
         TAC *aux_code = programa->code;
         aux_code = invertTacList(aux_code);
         assembly = concatTAC(assembly, aux_code);
      }


      programa = programa->child[1];
   }

   TAC *end = initTac();
   end->code = ILOC_HALT;
   end->labelName = (char *)malloc(sizeof(char));
   strcpy(end->labelName, "end");
   assembly = concatTAC(assembly, end);

   code = assembly;

   // Display the Assembly code
   printAssembly(code);
}

TAC *evaluateFinalTac(TAC *code, comp_list_t* declarations)
{
   TAC* aux_code = code;

   int fpPos = 0;
   while (aux_code != NULL)
   {
      if ((aux_code->code == ILOC_LOADAI) 
            && (aux_code->r1 == FP)
            && (aux_code->constant == -1))
      {
         aux_code->constant = fpPos;
         fpPos = fpPos + 4;

      }

      aux_code = aux_code->next;
   }  

   return code;
}

RA * calculateFrameSize(char* functionName, comp_tree_t* nodo, comp_list_t* declarations)
{
   RA *frame = (RA *)malloc(sizeof(RA));

   // Getting the first function declared in the AST
//   comp_tree_t* programa = nodo->child[0];

   // Navigating thru the functions
//   while (programa != NULL)
//   {
//      if (strcmp(programa->code->labelName, functionName))
//      {
         // Getting function details
         comp_list_t* listLocalDeclaration = getLocalDeclarations(functionName, declarations, IKS_LOCAL); 
         comp_list_t* listParametersDeclaration = getLocalDeclarations(functionName, declarations, IKS_FUNC_PARAM); 
   

         // Local variables
         int localContextSize = 0;
         int count = 0;
         while (listLocalDeclaration != NULL)
         {
            localContextSize = localContextSize + sizeDeclarations(listLocalDeclaration->tipoVar);
            count++;
            listLocalDeclaration = listLocalDeclaration->next;
         }
         frame->localVarSize = localContextSize;
         frame->localVarQuantity = count;
         frame->localVarPosition = 4 * count;

         // Parameters
         localContextSize = 0;
         count = 0;
         while (listParametersDeclaration != NULL)
         {
            localContextSize = localContextSize + sizeDeclarations(listParametersDeclaration->tipoVar);
            count++;
            listParametersDeclaration = listParametersDeclaration->next;
         }

         frame->paramSize = localContextSize;
         frame->paramQuantity = count;
         frame->paramPosition = 4 * count;

         // Return
         frame->returnSize = 4;

         // Static Link
         frame->staticLinkSize = 4;

         // Dynamic Link
         frame->dynamicLinkSize = 4;
//      }
//   }

   return frame;
}

char* getRegisterName(int regValue)
{
   char *valorStr = (char *)malloc(sizeof(char));
   switch (regValue)
   {
      case BSS:
         strcpy(valorStr, "bss");
         break;
      case FP:
         strcpy(valorStr, "fp");
         break;
      case SP:
         strcpy(valorStr, "sp");
         break;
      default:
         sprintf(valorStr, "r%d", regValue);
         break;
   }

   return valorStr;
}
