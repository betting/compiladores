/*
   main.c

   Arquivo principal do analisador sintático.
*/
#include "main.h"
#include "comp_tree.h"

extern int lineNumber;
extern FILE* yyin;
 
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
   if (argc > 1)
   {
      yyin  = fopen(argv[1], "r");
   }
   
   gv_init("saida.dot");

   int resultado = yyparse();
   gv_close();
   
   if (argc > 1)
   {
      fclose(yyin);
   }
   return resultado;
}

