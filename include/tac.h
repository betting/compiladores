#include <stdio.h>
#include <stdlib.h>
#include "iks_ast.h"
#include "comp_tree.h"
#include "comp_list.h"
#include "iloc.h"
#include "labelsnregisters.h"

#ifndef tac_H
#define tac_H


typedef struct tac{

        int r1;
        int r2;
        int r3;
        int label;
        int constant;
        int code;
        struct tac* next;        
        
}TAC;

TAC *code;

//registradores e labels
int reg;
int label;

TAC* initTac();
TAC* CodeGenerate(comp_tree_t* nodo,TAC* code, int iloc_code);
TAC* insertTAC(comp_tree_t* nodo);
TAC* Operator2(comp_tree_t* nodo, int operatorCode);
TAC* concatTAC(TAC* parent,TAC* child);

#endif
