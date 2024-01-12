#!/bin/bash
# $1 = N
# $2 = M
# $3 = K
# $4 = int1.txt
# $5 = int2.txt
# $6 = out.txt

# python3 MatrixGen2.py $1 $2 $3

python3 transpose.py $5 $2 $3
gcc P1.c -o P1.out -lpthread
gcc P2.c -o P2.out -lpthread
gcc scheduler.c -o scheduler.out -lpthread

./scheduler.out $1 $2 $3 $4 transpose.txt $6
# meld output.txt matrixres.txt
