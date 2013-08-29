#!/bin/bash
clear
echo "Executando testes..."
echo
echo "-------------------------------------------------"
echo "1. Comentarios"
cat tests/syntax/test01_comment.txt | ./main

echo "-------------------------------------------------"
echo "2. Variaveis"
cat tests/syntax/test02_var.txt | ./main

echo "-------------------------------------------------"
echo "3. Funcoes"
cat tests/syntax/test03_functions.txt | ./main

echo "-------------------------------------------------"
echo "4. Atribuicoes"
cat tests/syntax/test04_attrb.txt | ./main

echo "-------------------------------------------------"
echo "93. Funcoes (Erro esperado: linha 18)"
cat tests/syntax/test93_functions_err.txt | ./main

echo "-------------------------------------------------"
echo "94. Atribuicoes (Erro esperado: linha 9)"
cat tests/syntax/test94_attrb_err.txt | ./main

echo "-------------------------------------------------"
