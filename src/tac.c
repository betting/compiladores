#include "tac.h"

int varType;
int variableTypeGlobal;
int variableTypeLocal;

TAC* CodeGenerate(comp_tree_t* nodo,TAC* code, int iloc_code, comp_list_t* declarations, char *actualFunction)
{
   switch(iloc_code)
   {
      case ILOC_NOP:
         break;

      case ILOC_ADD:
         code = Operator2(nodo, ILOC_ADD);
         code = insertTAC(nodo);

         printLabel(code);
         printf("add r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_SUB:
         code = Operator2(nodo, ILOC_SUB);
         code = insertTAC(nodo);

         printLabel(code);
         printf("sub r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_MULT:
         code = Operator2(nodo, ILOC_MULT);
         code = insertTAC(nodo);

         printLabel(code);
         printf("mult r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_DIV:
         code = Operator2(nodo, ILOC_DIV);
         code = insertTAC(nodo);

         printLabel(code);
         printf("div r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
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

         printLabel(code);
         printf("and r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_ANDI:
         break;

      case ILOC_OR:
         code = Operator2(nodo, ILOC_OR);
         code = insertTAC(nodo);

         printLabel(code);
         printf("or r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_ORI:
         break;

      case ILOC_XOR:
         break;

      case ILOC_XORI:
         break;

      case ILOC_LOADI:
         nodo->code = initTac();
         reg = getLabelReg(reg);
         nodo->code->r3 = reg;
         nodo->code->constant = atoi(nodo->symbol->token);
         nodo->code->code = ILOC_LOADI;
         code = insertTAC(nodo);

         printLabel(code);
         printf("loadI %d => r%d\n",code->constant,code->r3);
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

         printLabel(code);
         printf("loadAI %s, %d => r%d\n", (varType == IKS_GLOBAL_VAR)?"global":"local", code->constant, code->r3);
         return code;
         break;

      case ILOC_LOADAO:
         break;

      case ILOC_CLOAD:
         break;

      case ILOC_CLOADAI:
         break;

      case ILOC_CLOADAO:
         break;

      case ILOC_STORE:
         nodo->code = initTac();
         nodo->code->r1 = nodo->child->sibling->code->r3;
         nodo->code->r3 = nodo->child->code->r3;
         nodo->code->code = ILOC_STORE;
         code = insertTAC(nodo);

         printLabel(code);
         printf("store r%d => r%d\n",code->r1,code->r3);
         return code;
         break;

      case ILOC_STOREAI:
         break;

      case ILOC_STOREAO:
         break;

      case ILOC_CSTORE:
         break;

      case ILOC_CSTOREAI:
         break;

      case ILOC_CSTOREAO:
         break;

      case ILOC_I2I:
         break;

      case ILOC_C2C:
         break;

      case ILOC_C2I:
         break;

      case ILOC_I2C:
         break;

      case ILOC_JUMPI:
         break;

      case ILOC_JUMP:
         break;

      case ILOC_CBR:
			printf("\nCBR ");
			nodo->code = initTac();
			nodo->code->r3 = nodo->child->code->r3;
			if(nodo->child->sibling!=NULL) printf("NOT NULL!!");
			InsertLabel(nodo->child->sibling);
			if(nodo->child->sibling->sibling == NULL)
			{
				nodo->child->sibling->sibling = (comp_tree_t*)malloc(sizeof(comp_tree_t));
				code = CodeGenerate(nodo, code, ILOC_NOP, NULL, NULL);
			}
			InsertLabel(nodo->child->sibling->sibling);
			/*nodo->code->l1 = label - 1; 
			nodo->code->l2 = label; 
			nodo->code->next = NULL;
			nodo->code->code = ILOC_CBR;
			if(nodo->child->sibling->sibling->sibling == NULL)
					code = combineCTE(nodo,CTE_IF);
			else{
					InsertLabel(nodo->child->sibling->sibling->sibling);
					code = CodeGenerate(nodo->child->sibling, code, ILOC_JUMPI, NULL, NULL);
					code = combineCTE(nodo,CTE_IF_ELSE);
			}*/
			printf("CBR r%d, %d => r%d\n",reg,code->constant,code->r3);
			return code;
         break;

      case ILOC_CMP_LT:
         code = Operator2(nodo, ILOC_CMP_LT);
         code = insertTAC(nodo);

         printLabel(code);
         printf("cmp_lt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_LE:
         code = Operator2(nodo, ILOC_CMP_LE);
         code = insertTAC(nodo);

         printLabel(code);
         printf("cmp_le r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_EQ:
         code = Operator2(nodo, ILOC_CMP_EQ);
         code = insertTAC(nodo);

         printLabel(code);
         printf("cmp_eq r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_GE:
         code = Operator2(nodo, ILOC_CMP_GE);
         code = insertTAC(nodo);

         printLabel(code);
         printf("cmp_ge r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_GT:
         code = Operator2(nodo, ILOC_CMP_GT);
         code = insertTAC(nodo);

         printLabel(code);
         printf("cmp_gt r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
         return code;
         break;

      case ILOC_CMP_NE:
         code = Operator2(nodo, ILOC_CMP_NE);
         code = insertTAC(nodo);

         printLabel(code);
         printf("cmp_ne r%d, r%d => r%d\n",code->r1,code->r2,code->r3);
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
   if(nodo->child != NULL)
   {
      concatTAC(nodo->code,nodo->child->code);
   }

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
   parent = aux_tac;
   return parent;
}

void InsertLabel(comp_tree_t* nodo)
{
   TAC* aux;
   //printf("\nLABEL 1");
   if(nodo->code != NULL)
      aux = nodo->code;
   //printCode(aux);
   //printf("\nLABEL 2");
//   printCode(aux);
//   int cont = 0;
   while(aux->next != NULL)
   {
//	  printf("\nNOT NULL");
//	  printCode(aux);
      aux = aux->next;
//      cont++;
      //if(cont==100) break;
   }
//   printf("\nLABEL 3");
   label = getLabelReg(label);
//   printf("\nlabel: %d\n", label);   
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
	
	switch(caseCTE)
	{	//if simples
		case CTE_IF:
				if(aux_nodo->child->sibling->sibling != NULL)
						nodo->code = aux_nodo->child->sibling->sibling->code;
				concatTAC(nodo->code,aux_nodo->child->sibling->code);
				concatTAC(nodo->code,aux);
				concatTAC(nodo->code,aux_nodo->child->code);
				return nodo->code;
			break;
		//if-else
		case CTE_IF_ELSE:
				nodo->code = aux_nodo->child->sibling->sibling->sibling->code;
				concatTAC(nodo->code,aux_nodo->child->sibling->sibling->code);
				concatTAC(nodo->code,aux_nodo->child->sibling->code);
				concatTAC(nodo->code,aux);
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
