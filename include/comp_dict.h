#ifndef _comp_dict_h
#define _comp_dict_h

#include <stdio.h>

struct _dic 
{
   int type;
   char *token;
   char *text;
   struct _dic *next;
};

typedef struct _dic comp_dict_item_t;

/**
 * Dictionary initalization.
 *
 * Perform a dictionary initialization following the size set in a constant.
 */
void initDict();

/**
 * Add new item in a Dictionary.
 *
 * Add new key/value in the Dictionary, according to the value specified.
 *
 * @param *key Dicitonary value/key (Unique value).
 * @param *text String to be added.
 * @return Dictionary item created (Key/Value).
 */
comp_dict_item_t* addDict(char *key, char *text, int type);

/**
 * Get item address in the dictionary.
 * 
 * Search for the text informed by the user in the dictionary, return the 
 * actual position of it.
 *
 * @param *text The content to searched.
 * @return The position of the content in the dictionary.
 */

int getItemAddressDict(char *text);

/**
 * Get an item from the dictionary.
 *
 * Search for the text informed by the user in the dictionary, returning
 * dictionary item.
 *
 * @param *text The content to searched.
 * @return The item content found.
 */
comp_dict_item_t* searchDict(char *text);

/**
 * Print Dictionary
 *
 * Print all elements added in the dictionary.
 */
void printDict();

/**
 * Remove the caracters \" and \' from a string type
 *
 * @param char* The value to remove the chars.
 * @return char* New value without the chars.
 */
char* convertString(char* value);
#endif
