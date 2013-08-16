#include <stdio.h>

typedef struct comp_dic_t {
        int token;
        char *text;
        struct comp_dic_t *next;
} comp_dic_item_t;

void dic_init();
comp_dic_item_t* dic_insert(int token, char *text);
int dic_address(char *text);
comp_dic_item_t* dic_find(char *text);
void dic_print();
