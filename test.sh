#!/bin/bash

echo "Executando testes..."
echo
echo "1. Comentarios"
cat tests/syntax/test01_comment.txt | ./main

echo "2. Variaveis"
cat tests/syntax/test02_var.txt | ./main

echo "3. Funcoes"
cat tests/syntax/test03_functions.txt | ./main

echo "4. Atribuicoes"
cat tests/syntax/test04_attrb.txt | ./main
