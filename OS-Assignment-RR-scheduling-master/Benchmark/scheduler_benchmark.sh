#!/bin/bash
# $1 = N
# $2 = M
# $3 = K
# $4 = Number of threads (same for P1 and P2 currently)


for n in 10 20 50 100 250 500 1000;
do
for m in 10 20 50 100 250 500 1000;
do
for k in 10 20 50 100 250 500 1000;
do

python3 MatrixGen2.py $n $m $k

python3 transpose.py matrix2.txt $m $k
gcc P1.c -o P1.out -lpthread
gcc P2.c -o P2.out -lpthread
gcc scheduler_bench.c -o scheduler_bench.out -lpthread

./scheduler_bench.out $n $m $k matrix1.txt transpose.txt output.txt
# meld output.txt matrixres.txt

done
done
done
