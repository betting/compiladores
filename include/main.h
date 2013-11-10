/*
  main.h

  Cabeçalho principal do analisador sintático
*/
#ifndef __MAIN_H
#define __MAIN_H
#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "labelreg.h"
#include "tac.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "stack.h"
#include "iks_ast.h"
#include "gv.h"



/*
  Constantes a serem utilizadas como valor de retorno no caso de
  sucesso (IKS_SYNTAX_SUCESS) e erro (IKS_SYNTAX_ERRO) do analisador
  sintático.
*/
#define IKS_SYNTAX_SUCESSO 0
#define IKS_SYNTAX_ERRO    1

/*
  Constantes a serem utilizadas para diferenciar os lexemas que estão
  registrados na tabela de símbolos.
*/
#define IKS_SIMBOLO_INT    1
#define IKS_SIMBOLO_FLOAT  2
#define IKS_SIMBOLO_CHAR   3
#define IKS_SIMBOLO_STRING 4
#define IKS_SIMBOLO_BOOL   5

/*
 Variáveis globais
*/

/**
 * Saves ast root in memory after parser
 *
 * @param *root
 * @return void
 */
void saveASTRoot(comp_tree_t* root);

/**
 * Check list of declarations
 *
 * @param *firt
 * @return void
 */
void saveDeclarationsList(comp_list_t* first);

/**
 * Returns head os the list
 *
 * @return comp_list_t
 */
comp_list_t* getListHead();

#endif
