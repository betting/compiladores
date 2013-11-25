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
//         code = insertTacEvaluated(nodo, code);

         //printCode(nodo->code);
         //printLabel(code);
         //printf("\nnop: %d\n", ILOC_NOP);

         return code;

         break;

      case ILOC_ADD:
         //printf("\nADD: %d\n", ILOC_ADD);
         //printCode(code);
      
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
         code = insertTAC(nodo);

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
            nodo->code->constant = sizeDeclarations(((variableTypeGlobal != -1) ? variableTypeGlobal : variableTypeLocal));
            nodo->code->code = ILOC_LOADAI;
            code = Address(nodo);
            
            code = insertTAC(nodo);
            //insertTAC(nodo);
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
         nodo->code->r1 = nodo->child[1]->code->r3;
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
//			printf("\nnodo->child->sibling->sibling nulo antes do insert \n ");
			InsertLabel(nodo->child[2]);

         // Updating parent node with children nodes.
			nodo->code->l1 = label-1;
			nodo->code->l2 = label; 
//			if(label>=1) printf("\n\nLABEL: %d", label);
			nodo->code->next = NULL;
			nodo->code->code = ILOC_CBR;

//			printf("\n\nCODE ANTES DE ENTRAR NO CTE");
//			printCode(nodo->code);
//			printf("cbr r%d => l%d, l%d\n", nodo->code->r3, nodo->code->l1, nodo->code->l2);

			//code = concatTAC(code,nodo->code);
			if(nodo->child[3] == NULL)
			{
//					printf("\nENTROU NO CTE");
					code = combineCTE(nodo,CTE_IF);
			}
			else
			{
//					printf("\nENTROU NO CTE - 2  ");
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
   //printf("======== INIT OPERATOR 2");
   nodo->code = initTac();
   //printf("======== END OPERATOR 2");
   //printCode(nodo->code);
   nodo->code->r1 = nodo->child[0]->code->r3;
   nodo->code->r2 = nodo->child[1]->code->r3;
   nodo->code->code = operatorCode;
   reg = getLabelReg(reg);
   //printf("======== END OPERATOR 2");
   nodo->code->r3 = reg;
   //printCode(nodo->code);
   
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
   if (nodo->child[3] != NULL)
   {
      concatTAC(nodo->code, nodo->child[3]->code);
   }

   if (nodo->child[2] != NULL)
   {
      concatTAC(nodo->code, nodo->child[2]->code);
   }

   if (nodo->child[1] != NULL)
   {
      concatTAC(nodo->code, nodo->child[1]->code);
   }

   if (nodo->child[0] != NULL)
   {
      concatTAC(nodo->code, nodo->child[0]->code);
   }

   /*
   int null = FALSE;
   int childExists_1 = FALSE;
   if(nodo->child != NULL)
   {
      childExists_1 = TRUE;
   }
   else
   {
      null = TRUE;
   }

   int childExists_2 = FALSE;
   if (null == FALSE)
   {
      if (nodo->child->sibling != NULL)
      {
         childExists_2 = TRUE;
      }
      else
      {
         null = TRUE;
      }
   }

   int childExists_3 = FALSE;
   if (null == FALSE)
   {
      if (nodo->child->sibling->sibling != NULL)
      {
         childExists_3 = TRUE;
      }
      else
      {
         null = TRUE;
      }
   }

   
   if (childExists_3 == TRUE)
   {
      concatTAC(nodo->code, nodo->child->sibling->sibling->code);
   }

   if (childExists_2 == TRUE)
   {
      concatTAC(nodo->code, nodo->child->sibling->code);
   }

   if (childExists_1 == TRUE)
   {
      concatTAC(nodo->code, nodo->child->code);
   }
   */
   
   return nodo->code;
   
}


TAC* concatTAC(TAC* parent,TAC* child)
{
   if ((parent != NULL) 
      && (child != NULL))
   {
//    int exists = FALSE;
      while(parent->next != NULL)
      {
//       if (child != aux_tac->next)
//       {
   	   parent = parent->next;
//       }
//       else
//       {
//          exists = TRUE;
//       }
      }
//    if (exists == FALSE)
//    {
      parent->next = child;
   }
   return parent;
}

TAC* insertTacEvaluated(comp_tree_t* nodo, TAC* code)
{
      concatTAC(nodo->code, nodo->child[1]->code);
      concatTAC(nodo->code, nodo->child[0]->code);
//      concatTAC(nodo->child[2]->code, nodo->code);
      concatTAC(code, nodo->code);
/*   
//   concatTAC(nodo->code, nodo->child->code);
   comp_tree_t* aux = getLastSibling(nodo->child);
   while (aux->child != NULL)
   {
      aux = getLastSibling(aux->child);
      if (countSiblings(aux) > 1)
      {
         concatTAC(nodo->code, aux->code);
      }
   }*/
   nodo->child[2]->code = code;
   return code;
//   return nodo->child[2]->code;
}

comp_tree_t* getLastSibling(comp_tree_t* nodo)
{
   /*
   while (nodo->sibling != NULL)
   {
      nodo = nodo->sibling;
   }

   return nodo;
//   return (nodo->sibling != NULL) ? nodo : NULL;
*/
}

int countSiblings(comp_tree_t* nodo)
{ /*
   int count = 0;
   nodo = nodo->child;
   if (nodo != NULL)
   {
      while (nodo->sibling != NULL)
      {
         count++;
         nodo = nodo->sibling;
      }
   }
   return count;
   */
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
//   printf("\nLABEL %d",label);
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

	
	switch(caseCTE)
	{	//if simples
		case CTE_IF:
//				printf("\n\nAUX DO CTE:");
//				printCode(aux);
            if(aux_nodo->child[2] != NULL)
            {
//			   printf("\n!=NULL\n");
               nodo->code = aux_nodo->child[2]->code;
            }

//             	printf("\n\nPÓS-IF");
//            	printCode(nodo->code);
            	           
				concatTAC(nodo->code,aux_nodo->child[1]->code);
//				printf("\n\nPÓS-CONCATTAC 1");
//            	printCode(nodo->code);

				concatTAC(nodo->code,aux);
//            	printCode(nodo->code);
//            	nodo->code = nodo->code->next;
//            	printCode(nodo->code);
//            	printCode(aux);			
				concatTAC(nodo->code,aux_nodo->child[0]->code);
//            	printf("\n\nNODO->CODE NO CTE");
//            	printCode(nodo->code);
				return nodo->code;
			break;
		//if-else
		case CTE_IF_ELSE:
				nodo->code = aux_nodo->child[3]->code;
				concatTAC(nodo->code,aux_nodo->child[2]->code);
//				concatTAC(nodo->code,aux_nodo->child->sibling->code);
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
   while(code != NULL)
   {
	  //printCode(code);
      switch(code->code)
      {
         case ILOC_NOP:
            printLabel(code);
            printf("nop\n");
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
            printf("loadai %s, %d => r%d\n", (code->r1 == BSS)?"bss":"fp", code->constant, code->r3);
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
		 /*while(lista_auxiliar->symbol->text=="nop")
		 {
			 printf("\nNOP encontrado");
			 lista_auxiliar=lista_auxiliar->next;
			 count++;
		 }
		 printf("\nQTD NOPS: %d", count);
		 lista_auxiliar = list;
		 
		 //procura valor nulo
		 while(lista_auxiliar->next != NULL)
			lista_auxiliar = lista_auxiliar->next;
		 
		 list = removeItem(searchToken(lista,lista_auxiliar->text));
		 lista_auxiliar = list;
		 */
	 }

	//tratar o resultado e devolver pro nodo
	return nodo->code;

}

TAC* CodeGenerateFuncDeclaration(comp_tree_t* novo, TAC* code, comp_list_t* declarations)
{
}

TAC* CodeGenerateFuncCall(comp_tree_t* nodo, TAC* code, comp_list_t* declarations)
{
   nodo->code = CodeGenerate(nodo, code, ILOC_NOP, NULL, NULL);
   nodo->code->code = ILOC_NOP;
//   code = insertTAC(nodo);
   int newLabel = getLabelReg(label);
   InsertLabel(nodo);

   TAC* aux_tac;

    // Updating FP (sub size)
    aux_tac = Operator2(nodo, ILOC_SUBI);
//    aux_tac->constant = size;
    aux_tac->r3 = FP;
    concatTAC(code, aux_tac);

    // Copying SP to FP
    aux_tac = Operator2(nodo, ILOC_I2I);
    aux_tac->r3 = FP;
    aux_tac->label = newLabel;
    concatTAC(code, aux_tac);

    // Jumping to function to be executed (Label)
    aux_tac = Operator2(nodo, ILOC_JUMPI);
//    aux_tac->l1 = getSize(???, declarationList); (Function name/size ???)
    concatTAC(code, aux_tac);

    // Saving return point on SP
    int newReg = getLabelReg(reg);
    aux_tac = Operator2(nodo, ILOC_STOREAI);
    aux_tac->constant = -4;
    aux_tac->r1 = newReg;
    concatTAC(code, aux_tac);

    // Loading actual label on register
    aux_tac = Operator2(nodo, ILOC_LOADI);
    aux_tac->constant = newLabel;
    aux_tac->r3 = newReg;
    concatTAC(code, aux_tac);

    // Moving pointer (SP)
    newReg = getLabelReg(reg);
    aux_tac = Operator2(nodo, ILOC_ADD);
    aux_tac->r2 = newReg;
    aux_tac->r3 = SP;
    concatTAC(code, aux_tac);

    // Loading size to register
    aux_tac = Operator2(nodo, ILOC_LOADI);
//    aux_tac->constant = size;
    aux_tac->r3 = newReg;
    concatTAC(code, aux_tac);

    // Updating FP with SP
    aux_tac = Operator2(nodo, ILOC_I2I);
    aux_tac->r3 = SP;
    concatTAC(code, aux_tac);
//    aux_tac->next = ; actual pointer

   return code; 
}

TAC* CodeGenerateReturn(comp_tree_t* nodo, TAC* code, comp_list_t* declarations)
{

}
