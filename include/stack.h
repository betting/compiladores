#ifndef _stack_h
#define _stack_h

#include "semantic.h"
#include <stdio.h>
#include "comp_tree.h"
#include "comp_list.h"

typedef struct stackTwo
{
  int size;
  int type;	//função, vet,...
  int tipoVar; //int, float,..
  char *nomeVar;
  struct stackTwo *previous;
  struct stackTwo *next;
}STACK2;

STACK2* pointerStack;

/**
 * Insert list in the top of the stack
 *
 * @param *list list of declarations
 * @return *stack with new list added
 */
STACK2* listToStack(comp_list_t* list);

/**
 * Add item to stack
 *
 * @param tipoVar type
 * @param *nomeVar
 * @param tipoGlobal
 * @param *node
 * @return stack
 */
STACK2* addItemStack(int tipoVar, char *nomeVar, int tipoGlobal, STACK2* node);

/**
 * Initialize stack
 * 
 * @return stack
 */
STACK2* initStack2();

/**
 * Add stack
 * 
 * @return stack
 */
STACK2* addStack(STACK2* stack_l, STACK2* node);

/**
 * Print stack
 * 
 * @return void
 */
void printStack2(STACK2* stack_l);
#endif
