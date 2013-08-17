#include <stdio.h>

struct _dic {
        int token;
        char *text;
        struct _dic *next;
};

typedef struct _dic comp_dic_item_t;

void dic_init();
comp_dic_item_t* dic_insert(int token, char *text);
int dic_address(char *text);
comp_dic_item_t* dic_find(char *text);
void dic_print();
