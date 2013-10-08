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
//comp_list_t *declarations;	

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem);
  printf ("Erro na linha: %d\n", getLineNumber());
}

void saveASTRoot(comp_tree_t* root)
{
	astRoot = root;
}

//void saveDeclarationsList(comp_list_t* first)
//{
	//declarations = first;
//}

//comp_list_t* getListHead()
//{
	//return declarations;
//}

int main (int argc, char **argv)
{
  gv_init("saida.dot");
  int resultado = yyparse();
  gv_close();
  return resultado;
}

