#ifndef _stack_h
#define _stack_h

#include "semantic.h"
#include <stdio.h>
#include "comp_tree.h"
#include "comp_list.h"

typedef struct stackTwo
{
  //comp_list_t *disc;
  int size;
  int type;	//função, vet,...
  int tipoVar; //int, float,..
  char *nomeVar;
  struct stackTwo *previous;
  struct stackTwo *next;
}STACK2;


STACK2* listToStack(comp_list_t* list);
STACK2* pointerStack;
STACK2* addItemStack(int tipoVar, char *nomeVar, int tipoGlobal, STACK2* node);
STACK2* initStack2();
STACK2* addStack(STACK2* stack_l, STACK2* node);
//insere no topo da pilha
void printStack2(STACK2* stack_l);
#endif