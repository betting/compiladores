#ifndef tac_H
#define tac_H


typedef struct tac{

        int r1;
        int r2;
        int r3;
        int label;
        int constant;
        int opcode;
        struct tac* next;        
        
}TAC;



#endif
