#!/bin/bash

DIR="tests/etapa05/"
for ARQUIVO in `ls -1 $DIR`
do
   echo $ARQUIVO
   ./main $DIR$ARQUIVO
done
