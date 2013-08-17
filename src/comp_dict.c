#include <stdlib.h>
#include <string.h>
#include "comp_dict.h"

#define SIZE 997

comp_dic_item_t *dicTable[SIZE];

void dic_init()
{
        int i;
        for(i = 0; i < SIZE; ++i)
                dicTable[i] = 0;
}

comp_dic_item_t* dic_insert(int token, char *text)
{
        int address;
        comp_dic_item_t *element;

        if(element = dic_find(text))
                return element;

        element = (HASH_ELEMENT*)malloc(sizeof(HASH_ELEMENT));
        element->token = token;
        element->text = (char*)calloc(strlen(text)+1,sizeof(char));
        strcpy(element->text, text);
        element->next = 0;

        address = dic_address(text);
        element->next = dicTable[address];
        dicTable[address] = element;

        return element;
}

int dic_address(char *text)
{
        int i;
        int address=1;
        int textlen = strlen(text);
        for(i = 0; i < textlen; ++i)
        {
                address = (address * text[i]) % SIZE + 1;
        }
        return address - 1;
}

comp_dic_item_t* dic_find(char *text)
{
        int address;
        comp_dic_item_t *ret;

        address = dic_address(text);

        ret = dicTable[address];
        while(ret && strcmp(ret->text, text) != 0) ret = ret->next;

        return ret;
}

void dic_print()
{
        int i;
        comp_dic_item_t *aux;
        for(i = 0; i < SIZE; ++i){
                aux = dicTable[i];
                while(aux != 0) {
                        printf("%d %s\n", aux->token, aux->text);
                        aux = aux->next;
                }
        }
}
