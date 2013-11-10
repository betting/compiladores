#include "tac.h"


TAC* CodeGenerate(comp_tree_t* nodo,TAC* code, int iloc_code)
{
   switch(iloc_code)
   {
      case ILOC_NOP:
         break;

      case ILOC_ADD:
         code = Operator2(nodo, ILOC_ADD);
         code = insertTAC(nodo);
         return code;
         break;

      case ILOC_SUB:
         break;

      case ILOC_MULT:
         break;

      case ILOC_DIV:
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
         break;

      case ILOC_ANDI:
         break;

      case ILOC_OR:
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
         return code;
         break;

      case ILOC_LOAD:
         break;

      case ILOC_LOADAI:
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
         nodo->code->r1 = nodo->child->sibling->child->code->r3;
         nodo->code->r3 = nodo->child->sibling->child->sibling->code->r3;
         nodo->code->code = ILOC_STORE;
         code = insertTAC(nodo);
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
         break;

      case ILOC_CMP_LT:
         break;

      case ILOC_CMP_LE:
         break;

      case ILOC_CMP_EQ:
         break;

      case ILOC_CMP_GE:
         break;

      case ILOC_CMP_GT:
         break;

      case ILOC_CMP_NE:
         break;
   }
}


TAC* Operator2(comp_tree_t* nodo, int operatorCode)
{
   nodo->code = initTac();
   comp_tree_t* currentChild;
   if(nodo->child!=NULL)
   {
      currentChild = nodo->child;
      printf("Operator2 CHILD eh != NULL");
      if(currentChild->code!=NULL) printf("Operator2 CODE eh != NULL");

//      printf("Operator2 CHILD - r3: %d",currentChild->code->r3);
   /*
      while(currentChild->sibling != NULL)
      {
         //faz algo
         printf("Operator2 SIBLING - r3: %d",currentChild->sibling->code->r3);
         currentChild = currentChild->sibling;
      }
      //if(nodo->symbol!=NULL)    printf("SYMBOL-TOKEN %s",nodo->symbol->token);
      if(nodo->child!=NULL)
      {
          printf("\nCHILD->token %s",nodo->child->symbol->token);
          if(nodo->child->code!=NULL)
         printf("\nCHILD-code->r3 %d",nodo->child->code->r3);
      }*/
   }
   /*if(nodo->sibling!=NULL)
   {
       printf("\nSIBLING->token %s",nodo->sibling->child->symbol->token);
       if(nodo->sibling->child->code!=NULL)
         printf("\nSIBLING-code->r3 %d",nodo->sibling->child->code->r3);
   }*/
   //struct tac* code;
   //comp_dict_item_t* symbol;
   //struct _tree *child,*sibling;

   //if(nodo->child!=NULL) printf("CHILD->CODE %d",nodo->child->code->r3);
   /*if(nodo->child!=NULL)
      nodo->code->r1 = nodo->child->code->r3;
   nodo->code->r2 = nodo->child->sibling->code->r3;
   reg = RegGenerator(reg);
   nodo->code->r3 = reg;
   nodo->code->code = operatorCode;*/
   return nodo->code;
}

TAC* initTac()
{
   TAC* new =(TAC*)malloc(sizeof(TAC));
   new->r1 = 0;
   new->r2 = 0;
   new->r3 = 0;
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
   while(aux_tac->next!= NULL)
   {
      aux_tac = aux_tac->next;
   }
   aux_tac->next = child;
   return parent;
}
