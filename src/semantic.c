/**
 * @file semantic.c
 * @version 1.0
 *
 * @section DESCRIPTION
 * Semantic Analisys
 * Perform a semantic analisys in a code provided by the user
 */

#include <stdlib.h>
#include "comp_tree.h"
#include "iks_ast.h"
#include "semantic.h"

/**
 * Check Declarations.
 * 
 * Check all variable and function declarations.
 *
 * @param *root   The tree with all elements found
 * @return The return code (sucess or error)
 */
int checkDeclarations(comp_tree_t *root)
{
   // Checking if there are variable/vector declariation or function definition.
   if(root->type == IKS_AST_FUNCAO)
   {
      if(root->symbol == 0)
      {
         // Identifier undeclared
         return IKS_ERROR_UNDECLARED;
      }
      else
      {
         if (root->symbol->type != IKS_AST_IDENTIFICADOR)
         {
            // Symbol already definied
            return IKS_ERROR_DECLARED;
         }
      }
   }
}
