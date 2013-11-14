#ifndef ILOC_H
#define ILOC_H
#define ILOC_NOP        0
//aritmética
#define ILOC_ADD        1
#define ILOC_SUB        2
#define ILOC_MULT        3
#define ILOC_DIV        4
#define ILOC_ADDI        5
#define ILOC_SUBI        6
#define ILOC_RSUBI        7
#define ILOC_MULTI        8
#define ILOC_DIVI        9
#define ILOC_RDIVI        10
//shifts
#define ILOC_LSHIFT        11
#define ILOC_LSHIFTI        12
#define ILOC_RSHIFT        13
#define ILOC_RSHIFTI        14

#define ILOC_AND        15
#define ILOC_ANDI        16
#define ILOC_OR                17
#define ILOC_ORI        18
#define ILOC_XOR        19
#define ILOC_XORI        20
//operações sobre memória
#define ILOC_LOADI        21
#define ILOC_LOAD        22
#define ILOC_LOADAI        23
#define ILOC_LOADAO        24
#define ILOC_CLOAD        25
#define ILOC_CLOADAI        26
#define ILOC_CLOADAO        27
#define ILOC_STORE        28
#define ILOC_STOREAI        29
#define ILOC_STOREAO        30
#define ILOC_CSTORE        31
#define ILOC_CSTOREAI        32
#define ILOC_CSTOREAO        33
#define ILOC_I2I        34
#define ILOC_C2C        35
#define ILOC_C2I        36
#define ILOC_I2C        37
//operações ILOC de fluxo de controle
#define ILOC_JUMPI        38
#define ILOC_JUMP        39
#define ILOC_CBR        40
#define ILOC_CMP_LT        41
#define ILOC_CMP_LE        42
#define ILOC_CMP_EQ        43
#define ILOC_CMP_GE        44
#define ILOC_CMP_GT        45
#define ILOC_CMP_NE        46

#define ILOC_CBR_WHILE		99
#define ILOC_CBR_DO			999


#endif
