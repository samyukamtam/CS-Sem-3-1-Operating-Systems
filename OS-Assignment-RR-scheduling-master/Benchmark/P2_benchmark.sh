gcc P1.c -o P1.out -lpthread
gcc P2_bench.c -o P2_bench.out -lpthread
python3 transpose.py $5 $2 $3
./P1.out $1 $2 $3 $4 transpose.txt $6 12
./P2_bench.out $1 $2 $3