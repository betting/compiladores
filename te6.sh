#!/bin/bash

DIR="tests/etapa06/input/"
for ARQUIVO in `ls -1 $DIR`
do
   echo $ARQUIVO
   ./main $DIR$ARQUIVO
done
