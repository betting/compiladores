#include "tac.h"


TAC* CodeGenerate(comp_tree_t* nodo,TAC* code, int iloc_code)
{

	switch(iloc_code)
	{
		case ILOC_NOP:
				break;
		case ILOC_ADD:
					code = Operator2(nodo, ILOC_ADD);
					//code = insertTAC(nodo);
					return code;
				break;
				
		case ILOC_SUB:
		case ILOC_MULT:
		case ILOC_DIV:
		case ILOC_ADDI:
		case ILOC_SUBI:
		case ILOC_RSUBI:
		case ILOC_MULTI:
		case ILOC_DIVI:
		case ILOC_RDIVI:
		case ILOC_LSHIFT:
		case ILOC_LSHIFTI:
		case ILOC_RSHIFT:
		case ILOC_RSHIFTI:
		case ILOC_AND:
		case ILOC_ANDI:
		case ILOC_OR:
		case ILOC_ORI:
		case ILOC_XOR:
		case ILOC_XORI:
		case ILOC_LOADI:
		case ILOC_LOAD:
		case ILOC_LOADAI:
		case ILOC_LOADAO:
		case ILOC_CLOAD:
		case ILOC_CLOADAI:
		case ILOC_CLOADAO:
		case ILOC_STORE:
		case ILOC_STOREAI:
		case ILOC_STOREAO:
		case ILOC_CSTORE:
		case ILOC_CSTOREAI:
		case ILOC_CSTOREAO:
		case ILOC_I2I:
		case ILOC_C2C:
		case ILOC_C2I:
		case ILOC_I2C:
		case ILOC_JUMPI:
		case ILOC_JUMP:
		case ILOC_CBR:
		case ILOC_CMP_LT:
		case ILOC_CMP_LE:
		case ILOC_CMP_EQ:
		case ILOC_CMP_GE:
		case ILOC_CMP_GT:
		case ILOC_CMP_NE:
					break;
	}


}


TAC* Operator2(comp_tree_t* nodo, int operatorCode)
{
	nodo->code = initTac();
	printf("\n\n\n\n================ETAPA 5==========================");
	printf("\nType: %d", nodo->type);
	printf("\nNode Type: %d", nodo->node_type);
	printf("\nType: %d", nodo->type);
	printf("\nSize: %d", nodo->size);
	//if(nodo->symbol!=NULL) 	printf("SYMBOL-TOKEN %s",nodo->symbol->token);
	if(nodo->child!=NULL)
	{
		 printf("\nCHILD->token %s",nodo->child->symbol->token);
		 if(nodo->child->code!=NULL)
			printf("\nCHILD-code->r3 %d",nodo->child->code->r3);
	}

	if(nodo->sibling!=NULL)
	{
		 printf("\nSIBLING->token %s",nodo->sibling->child->symbol->token);
		 if(nodo->sibling->child->code!=NULL)
			printf("\nSIBLING-code->r3 %d",nodo->sibling->child->code->r3);
	}
	//struct tac* code;
	//comp_dict_item_t* symbol;
	//struct _tree *child,*sibling;
	
	//if(nodo->child!=NULL) printf("CHILD->CODE %d",nodo->child->code->r3);
	/*if(nodo->child!=NULL)
		nodo->code->r1 = nodo->child->code->r3;
	/*nodo->code->r2 = nodo->child->sibling->code->r3;
	reg = RegGenerator(reg);
	nodo->code->r3 = reg;
	nodo->code->code = operatorCode;*/
	return nodo->code;
}

TAC* initTac()
{
	TAC* new =(TAC*)malloc(sizeof(TAC));
	new->r1=0;
	new->r2=0;
	new->r3=0;
	new->r1=0;
	new->label=0;
	new->constant=0;
	new->code = 0;
	new->next = NULL;
	return new;
}


TAC* insertTAC(comp_tree_t* nodo)
{
	//montar a inserção do TAC nos nodos		
	if(nodo->child != NULL)
		concatTAC(nodo->code,nodo->child->code);
    return nodo->code;
}


TAC* concatTAC(TAC* parent,TAC* child){
	TAC* aux_tac = parent;
	while(aux_tac->next!= NULL)
			aux_tac = aux_tac->next;
	aux_tac->next = child;
	return parent;
}
