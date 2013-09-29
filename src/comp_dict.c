/**
 * @file comp_dict.c 
 * @version 1.0
 *
 * @section DESCRIPTION
 * Dictionary
 * A data type with a table (comp_dict_t) and one value/item (comp_dict_item_t).
 */

#include <stdlib.h>
#include <string.h>
#include "comp_dict.h"

#define SIZE 997

comp_dict_item_t *dicTable[SIZE];

/**
 * Dictionary initalization.
 *
 * Perform a dictionary initialization following the size set in a constant.
 */
void initDict()
{
   int i;
   for(i = 0; i < SIZE; ++i)
   {
      dicTable[i] = 0;
   }
}

/**
 * Add new item in a Dictionary.
 *
 * Add new key/value in the Dictionary, according to the value specified.
 *
 * @param  key    Dicitonary value/key (Unique value).
 * @param  *text  String to be added.
 * @param  type   Type of identifiers/literal 
 * @return Dictionary item created (Key/Value).
 */
comp_dict_item_t* addDict(char *key, char *text, int type)
{
   int address;
   comp_dict_item_t *element;

   // Since we should only have unique key in the dictionary, once the 
   // key informed by the user is found, the previous (first) element
   // created in the dicionary will be returned.
//   if(element = searchDict(text))
//   {
//      return element;
//   }

   // Creating new dictionary item
   element = (comp_dict_item_t *)malloc(sizeof(comp_dict_item_t));

   // Adding new elements to it.
   element->token = (char*)calloc(strlen(key)+1,sizeof(char));
   strcpy(element->token, key);

   element->text = (char*)calloc(strlen(text)+1,sizeof(char));
   strcpy(element->text, text);
   
   element->type = type;
   
   element->next = 0;

   // Getting references from global dictionary to add new item to it.
   address = getItemAddressDict(text);
   element->next = dicTable[address];
   dicTable[address] = element;

   return element;
}

/**
 * Get item address in the dictionary.
 * 
 * Search for the text informed by the user in the dictionary, return the 
 * actual position of it.
 *
 * @param *text The content to searched.
 * @return The position of the content in the dictionary.
 */
int getItemAddressDict(char *text)
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

/**
 * Get an item from the dictionary.
 *
 * Search for the text informed by the user in the dictionary, returning
 * dictionary item.
 *
 * @param *text The content to searched.
 * @return The item content found.
 */
comp_dict_item_t* searchDict(char *text)
{
   int address;
   comp_dict_item_t *ret;

   address = getItemAddressDict(text);

   ret = dicTable[address];
   while(ret && strcmp(ret->text, text) != 0) 
   {
      ret = ret->next;
   }

   return ret;
}

/**
 * Print Dictionary
 *
 * Print all elements added in the dictionary.
 */
void printDict()
{
   int i;
   comp_dict_item_t *aux;
   for(i = 0; i < SIZE; ++i)
   {
      aux = dicTable[i];
      while(aux != 0) 
      {
         printf("%s %s\n", aux->token, aux->text);
         aux = aux->next;
      }
   }
}


/**
 * Remove the characters \" and \' from a string type
 *
 * @param char* The value to remove the chars.
 * @return char* New value without the chars.
 */
char* convertString(char* value)
{
   char * newString = (char*)calloc(strlen(value)+1,sizeof(char));

   strncpy(newString, value + 1, strlen(value) - 2);
//   printf("%s\n", value);
//   printf("%s\n", newString);

   return newString;
}
