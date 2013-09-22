/*
   main.c

   Arquivo principal do analisador sintático.
*/
#include "main.h"
#include "comp_tree.h"

extern int lineNumber;
 
int getLineNumber (void)
{
   return lineNumber;
}

comp_tree_t* astRoot;

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem);
  printf ("Erro na linha: %d\n", getLineNumber());
}

void saveASTRoot(comp_tree_t* root)
{
	astRoot = root;
}

int main (int argc, char **argv)
{
  gv_init("saida.dot");
  int resultado = yyparse();
  gv_close();
  return resultado;
}

