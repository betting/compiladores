/*
   main.c

   Arquivo principal do analisador sintático.
*/
#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"

#define IKS_SYNTAX_SUCESSO	0
#define IKS_SYNTAX_ERRO		1

extern int lineNumber;
 
int getLineNumber (void)
{
   return lineNumber;
}

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem);
  printf ("Erro na linha: %d\n", getLineNumber());
  exit(IKS_SYNTAX_ERRO);
}

int main (int argc, char **argv)
{
  int resultado = yyparse();
  exit(IKS_SYNTAX_SUCESSO);
}
