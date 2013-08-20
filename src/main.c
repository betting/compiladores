#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

extern int lineNumber;

int getLineNumber (void)
{
  return lineNumber;
}

int main (int argc, char **argv)
{
  initDict();
  int token = TOKEN_ERRO;
  while (token = yylex())
  {
    printf ("token <%d> at %d\n", token, getLineNumber());
  }
  return 0;
}
