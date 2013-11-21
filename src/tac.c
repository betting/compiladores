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
         code = insertTAC(nodo);

         printLabel(code);
//         printf("nop\n");

         return code;
         break;

      case ILOC_ADD:
         code = Operator2(nodo, ILOC_ADD);
         code = insertTAC(nodo);

//         printLabel(code);
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
         code = insertTAC(nodo);

//         printLabel(code);
//         printf("loadI %d => r%d\n",code->constant,code->r3);
         return code;
         break;

      case ILOC_LOAD:
         break;

      case ILOC_LOADAI:
         if (nodo->type == IKS_AST_IDENTIFICADOR)
         {
            variableTypeGlobal = getDeclarationDataType(IKS_GLOBAL_VAR, nodo->symbol->token, declarations, NULL);
            if (variableTypeGlobal == -1)
            {
               variableTypeLocal = getDeclarationDataType(IKS_LOCAL, nodo->symbol->token, declarations, actualFunction);
               varType = IKS_LOCAL;
            }
            else
            {
               varType = IKS_GLOBAL_VAR;
            }

            nodo->code = initTac();
            reg = getLabelReg(reg);
            nodo->code->r1 = varType;
            nodo->code->r3 = reg;
            nodo->code->constant = sizeDeclarations(((variableTypeGlobal != -1) ? variableTypeGlobal : variableTypeLocal));
            nodo->code->code = ILOC_LOADAI;
            code = insertTAC(nodo);
         }

//         printLabel(code);
//         printf("loadAI %s, %d => r%d\n", (varType == IKS_GLOBAL_VAR)?"global":"local", code->constant, code->r3);
         return code;
         break;


      case ILOC_STORE:
         nodo->code = initTac();
         nodo->code->r1 = nodo->child->sibling->code->r3;
         nodo->code->r3 = nodo->child->code->r3;
         nodo->code->code = ILOC_STORE;
         code = insertTAC(nodo);

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
			nodo->code = initTac();
			
			nodo->code->r3 = nodo->child->code->r3;
			if(nodo->child->sibling!= NULL)
				InsertLabel(nodo->child->sibling);
			//1 - verificar se tem irmãos
			/*int count = -1;
			if(nodo->child != NULL)
			{
				count++;
				currentNodo = nodo->child;
				printf("\n%d child", count);
			}
			else
			{
				count = 99;
				printf("\n COUNT 99 do CHILD");
			}
			
			while(count != 99)
			{
				if(currentNodo->sibling != NULL)
				{
					count++;
					printf("\nIrmão %d ", count);
					if(count == 1)
					{
						InsertLabel(nodo->child->sibling);
					}
					if(count == 2)
					{
						nodo->child->sibling->sibling = (comp_tree_t*)malloc(sizeof(comp_tree_t));
						code = CodeGenerate(nodo->child->sibling->sibling, code, ILOC_NOP, NULL, NULL);
					}
					if(count == 3)
					{
						InsertLabel(nodo->child->sibling);
					}
					currentNodo = currentNodo->sibling;
				}
				else
				{
					count = 99;
					printf("\n COUNT 99 dos IRMAOS");
				}
			}
			*/
			if(nodo->child->sibling->sibling == NULL)
			{
//				printf("\nnodo->child->sibling->sibling eh nulo ");
				
				nodo->child->sibling->sibling = (comp_tree_t*)malloc(sizeof(comp_tree_t));
				code = CodeGenerate(nodo->child->sibling->sibling, code, ILOC_NOP, NULL, NULL);
			}
//			printf("\nnodo->child->sibling->sibling nulo antes do insert \n ");
			InsertLabel(nodo->child->sibling->sibling);
//			printf("\nPassou o InsertLabel dos 2 irmãos\n ");
			nodo->code->l1 = label - 1; 
			nodo->code->l2 = label; 
			nodo->code->next = NULL;
			nodo->code->code = ILOC_CBR;
//			printf("\nANTES DO IF FINAL\n");
			if(nodo->child->sibling->sibling->sibling == NULL)
			{
//					printf("\nENTROU NO CTE");
					code = combineCTE(nodo,CTE_IF);
			}
			else
			{
//					printf("\nENTROU NO CTE - 2  ");
					InsertLabel(nodo->child->sibling->sibling->sibling);
					code = CodeGenerate(nodo->child->sibling, code, ILOC_JUMPI, NULL, NULL);
					code = combineCTE(nodo,CTE_IF_ELSE);
			}
//			printf("\nPassou o IF FINAL\n");
//			printf("cbr r%d => l%d, l%d\n", nodo->code->r3, nodo->code->l1, nodo->code->l2);
			return code;
			
         break;
      
      case ILOC_CBR_WHILE:
			jump = initTac();
			jump->code = ILOC_JUMPI;
			nodo->code = initTac();
			nodo->code->r3 = nodo->child->code->r3;
			if(nodo->child->sibling->child == NULL)
			{
//				nodo->child->sibling->sibling = (comp_tree_t*)malloc(sizeof(comp_tree_t));
				nodo->child->sibling->child = (comp_tree_t*)malloc(sizeof(comp_tree_t));
				code = CodeGenerate(nodo->child->sibling->sibling, code, ILOC_NOP, NULL, NULL);
			}
			label = getLabelReg(label);
			nodo->child->code->label = label;
			jump->l1 = label;
			InsertLabel(nodo->child->sibling);
			InsertLabel(nodo->child->sibling->child);
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
				nodo->code->r3 = nodo->child->child->code->r3;
			else
				nodo->code->r3 = nodo->child->code->r3;
			nodo->code->l1 = label;
			if(nodo->child->sibling->child == NULL)
			{
				nodo->child->sibling->child = (comp_tree_t*)malloc(sizeof(comp_tree_t));
				code = CodeGenerate(nodo->child->sibling->child, code, ILOC_NOP, NULL, NULL);
			}
			InsertLabel(nodo->child->sibling->child);
			nodo->code->l2 = label; 
			aux_tac = nodo->code;
			nodo->code = jump;
			nodo->code->next = aux_tac;
			nodo->code = insertTAC(nodo);
			return nodo->code;
        break; 

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
   nodo->code->r1 = nodo->child->code->r3;
   nodo->code->r2 = nodo->child->sibling->code->r3;
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
   //montar a inserção do TAC nos nodos
   int null = FALSE;
   //TAC* newCode = NULL;
   TAC* newCode = initTac();
   if(nodo->child != NULL)
   {
//      concatTAC(nodo->code, nodo->child->code);
      concatTAC(newCode, nodo->child->code);
   }
   else
   {
      null = TRUE;
   }

   if (null == FALSE)
   {
      if (nodo->child->sibling != NULL)
      {
//         concatTAC(nodo->code, nodo->child->sibling->code);
         concatTAC(newCode, nodo->child->sibling->code);
      }
      else
      {
         null = TRUE;
      }
   }

   if (null == FALSE)
   {
      if (nodo->child->sibling->sibling != NULL)
      {
//         concatTAC(nodo->code, nodo->child->sibling->sibling->code);
         concatTAC(newCode, nodo->child->sibling->sibling->code);
      }
      else
      {
         null = TRUE;
      }
   }

   if (null == FALSE)
   {
      if (nodo->child->sibling->sibling->sibling != NULL)
      {
//         concatTAC(nodo->code, nodo->child->sibling->sibling->sibling->code);
         concatTAC(newCode, nodo->child->sibling->sibling->sibling->code);
      }
      else
      {
         null = TRUE;
      }
   }

   newCode = invertTacList(newCode);
   concatTAC(nodo->code, newCode);
   return nodo->code;
}


TAC* concatTAC(TAC* parent,TAC* child)
{
   TAC* aux_tac = parent;
   while(aux_tac->next != NULL)
   {
	   aux_tac = aux_tac->next;
   }
   aux_tac->next = child;
   return parent;
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
   if(code->label != 0) 
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
	
	//printf("\nCASE CTE: %d",caseCTE);
	//printCode(aux);
	
	switch(caseCTE)
	{	//if simples
		case CTE_IF:
            if(aux_nodo->child->sibling->sibling != NULL)
            {
			   //printf("\n!=NULL\n");
               nodo->code = aux_nodo->child->sibling->sibling->code;
            }
               //printf("\nNODO CODE");
            
            	//printCode(aux_nodo->child->sibling->child->code);
			
               //printf("\nCONCATS...");
            	concatTAC(nodo->code,aux_nodo->child->sibling->code);
            	//printf("\nCONCATS - 1");
            	//printCode(nodo->code);
				concatTAC(nodo->code,aux);
				//printf("\nCONCATS - 2");
            	//printCode(nodo->code);
				concatTAC(nodo->code,aux_nodo->child->code);
				//printf("\nCONCATS - 3");
            	//printCode(nodo->code);
				return nodo->code;
			break;
		//if-else
		case CTE_IF_ELSE:
				nodo->code = aux_nodo->child->sibling->sibling->sibling->code;
				concatTAC(nodo->code,aux_nodo->child->sibling->sibling->code);
//				concatTAC(nodo->code,aux_nodo->child->sibling->code);
//				concatTAC(nodo->code,aux);
				concatTAC(nodo->code,aux_nodo->child->code);
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
   while(code != NULL)
   {
	  printCode(code);
      switch(code->code)
      {
         case ILOC_NOP:
            printLabel(code);
            break;
         case ILOC_ADD:
            printLabel(code);
            printf("add r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_SUB:
            printLabel(code);
            printf("sub r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_MULT:
            printLabel(code);
            printf("mult r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_DIV:
            printLabel(code);
            printf("div r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_ADDI:
            printLabel(code);
            break;
         case ILOC_SUBI:
            printLabel(code);
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
            printf("and r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_ANDI:
            printLabel(code);
            break;
         case ILOC_OR:
            printLabel(code);
            printf("or r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
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
            printf("loadi %d => r%d\n", code->constant, code->r3);
            break;
         case ILOC_LOAD:
            printLabel(code);
            break;
         case ILOC_LOADAI:
            printLabel(code);
            printf("loadai %s, %d => r%d\n", (code->r1 == IKS_GLOBAL_VAR)?"bss":"fp", code->constant, code->r3);
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
            printf("store r%d => r%d\n",code->r1,code->r3);
            break;
         case ILOC_STOREAI:
            printLabel(code);
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
            printLabel(code);
   			printf("jumpI => l%d\n", code->label);
            break;
         case ILOC_JUMP:
            printLabel(code);
            break;
         case ILOC_CBR:
            printLabel(code);
   			printf("cbr r%d => l%d, l%d\n", code->r3, code->l1, code->l2);
            break;
         case ILOC_CMP_LT:
            printLabel(code);
            printf("cmp_lt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_LE:
            printLabel(code);
            printf("cmp_le r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_EQ:
            printLabel(code);
            printf("cmp_eq r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_GE:
            printLabel(code);
            printf("cmp_ge r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_GT:
            printLabel(code);
            printf("cmp_gt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
            break;
         case ILOC_CMP_NE:
            printLabel(code);
            printf("cmp_ne r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
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
