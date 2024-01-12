import sys

n = int(sys.argv[2])
m = int(sys.argv[3])

file = open(sys.argv[1], 'r')
matrix = [list() for _ in range(n)]
cnt = 0
for i in file.readlines():
    matrix[cnt] = list(map(int, i.split()))
    cnt += 1
file.close()

file = open('transpose.txt', 'w')
ans = list()
for i in range(m):
    now = ''
    for j in range(n):
        now += str(matrix[j][i]) + ' '
    now += '\n'
    ans.append(now)

file.writelines(ans)
file.close()
