#include <stdio.h>
#include <stdlib.h>
#include "iks_ast.h"
//#include "Registers.h"
#include "comp_tree.h"
#include "comp_list.h"
#include "iloc.h"

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

TAC* initTac();
TAC* CodeGenerate(comp_tree_t* nodo,TAC* code, int iloc_code);
TAC* insertTAC(comp_tree_t* nodo);
TAC* Operator2(comp_tree_t* nodo, int operatorCode);

#endif
