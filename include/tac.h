#include <stdio.h>
#include <stdlib.h>
#include "iks_ast.h"
#include "comp_tree.h"
#include "comp_list.h"
#include "iloc.h"
#include "labelreg.h"

#ifndef tac_H
#define tac_H

#define CTE_IF 1
#define CTE_IF_ELSE 2
#define CTE_WHILE 3


typedef struct tac
{
   int r1;
   int r2;
   int r3;
   int l1;
   int l2;
   int label;
   int constant;
   int code;
   struct tac* next;
} TAC;

TAC *code;

//registradores e labels
int reg;
int label;
char *actualFunction;

TAC* initTac();
TAC* CodeGenerate(comp_tree_t* nodo,TAC* code, int iloc_code, comp_list_t* declarations, char *actualFunction);
TAC* insertTAC(comp_tree_t* nodo);
TAC* Operator2(comp_tree_t* nodo, int operatorCode);
TAC* concatTAC(TAC* parent,TAC* child);
TAC* combineCTE(comp_tree_t* nodo, int caseCTE);
void printLabel(TAC* code);
void InsertLabel(comp_tree_t* nodo);
void printCode(TAC* code);
TAC* invertTacList(TAC* list);
TAC* insertTacEvaluated(comp_tree_t* nodo, TAC* code);
comp_tree_t* getLastSibling(comp_tree_t* nodo);
int countSiblings(comp_tree_t* nodo);
#endif
