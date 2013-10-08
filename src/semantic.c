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




listaVar *addList(listaVar *varDeclaration, int tipoVar, char *nomeVar)
{
   printf("addList");
   varDeclaration = malloc(sizeof(listaVar));
   varDeclaration->tipoVar = tipoVar;
   varDeclaration->nomeVar = nomeVar;
   printf("\ntipo: %d id: %s\n", varDeclaration->tipoVar, varDeclaration->nomeVar);
   
   varDeclaration->nextList = NULL;
   return varDeclaration;
}


void listar(listaVar *varDeclaration)
{
   printf("\n\nLISTAR");
    
    int i=0;
    while( varDeclaration != NULL)    /* Enquanto nao chega no fim da lista */
    {
        i++;
	printf("DENTRO DO WHILE");
        printf("\n\nTipoVar %d - NomeVar: %s \n", varDeclaration->tipoVar, varDeclaration->nomeVar);
        varDeclaration = varDeclaration->nextList;     /* Faz noatual apontar para o proximo no */
    }
}



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
