#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>    
#include <time.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <assert.h> 
#define NANO 1E9

typedef long long ll;

int N, M, K;

typedef struct {
    int col_cnt; // number of columns in matrix
    int row_from, row_to; // row number
    char* path; // file path
    ll* offset; // offset of rows in file
    ll* mat; // stores current matrix
    bool* flag; // set flag after reading the row
} thread_params;

bool* flag1, * flag2;
ll* mat1, * mat2;

void pre_process_input(int N, int M, ll* offset, FILE* fp) {
    for (int i = 0; i < N; ++i) {
        char* line;
        size_t sz = 0;
        getline(&line, &sz, fp);
        if (i + 1 < N)
            offset[i + 1] = offset[i] + strlen(line);
    }
    fclose(fp);
}

// Threading function
void* read_file(void* args) {
    thread_params* P = (thread_params*)args;
    int start = P->row_from, end = P->row_to;
    int M = P->col_cnt;
    bool* flag = P->flag;
    FILE* fp = fopen(P->path, "r");
    ll* offset = P->offset;
    ll* mat = P->mat;

    for (int i = start; i <= end; ++i) {
        fseek(fp, offset[i], SEEK_SET);
        for (int j = 0; j < M; ++j)
            fscanf(fp, "%lld", &mat[i * M + j]);
        flag[i] = true;
    }

    fclose(fp);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Invalid input!!!\n");
        exit(0);
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    K = atoi(argv[3]);
    FILE* fp1 = fopen(argv[4], "r");
    FILE* fp2 = fopen(argv[5], "r");

    ll* offset_1 = malloc(N * sizeof(ll));
    ll* offset_2 = malloc(K * sizeof(ll));

    int mat1_id = shmget(1080, (N * M) * sizeof(ll), 0666 | IPC_CREAT);
    int mat2_id = shmget(153, (K * M) * sizeof(ll), 0666 | IPC_CREAT);
    int flag1_id = shmget(1892, (N) * sizeof(bool), 0666 | IPC_CREAT);
    int flag2_id = shmget(2068, (K) * sizeof(bool), 0666 | IPC_CREAT);

    // Attatching ids
    mat1 = shmat(mat1_id, NULL, 0);
    mat2 = shmat(mat2_id, NULL, 0);
    flag1 = shmat(flag1_id, NULL, 0);
    flag2 = shmat(flag2_id, NULL, 0);

    // Generating benchmark csv
    FILE* csv_ptr = fopen("P1_data.csv", "w");
    fprintf(csv_ptr, "No.of threads, Time\n");

    // Preprocess in1.txt
    pre_process_input(N, M, offset_1, fp1);
    // Preprocess in2.txt
    pre_process_input(K, M, offset_2, fp2);
    for (int MAX_THREADS = 1; MAX_THREADS <= N; ++MAX_THREADS) {
        struct timespec start_read, end_read;
        pthread_t threads[MAX_THREADS];

        clock_gettime(CLOCK_REALTIME, &start_read);
        if (MAX_THREADS == 1) { // Sequential Read 
            thread_params* cur = (thread_params*)(malloc(sizeof(thread_params)));
            cur->offset = offset_1;
            cur->path = argv[4];
            cur->row_from = 0;
            cur->row_to = N - 1;
            cur->col_cnt = M;
            cur->mat = mat1;
            cur->flag = flag1;
            pthread_create(&threads[0], NULL, read_file, (void*)cur);
            pthread_join(threads[0], NULL);

            cur->offset = offset_2;
            cur->path = argv[5];
            cur->row_from = 0;
            cur->row_to = K - 1;
            cur->col_cnt = M;
            cur->mat = mat2;
            cur->flag = flag2;

            pthread_create(&threads[0], NULL, read_file, (void*)cur);
            pthread_join(threads[0], NULL);
        } else {
            int MAX_THREADS1 = MAX_THREADS / 2;
            for (int i = 0; i < MAX_THREADS1; ++i) {
                thread_params* cur = (thread_params*)(malloc(sizeof(thread_params)));

                cur->offset = offset_1;
                cur->path = argv[4];
                cur->row_from = i * N / MAX_THREADS1;
                cur->row_to = (i + 1) * N / MAX_THREADS1 - 1;
                if (i == MAX_THREADS1 - 1) cur->row_to = N - 1;
                cur->col_cnt = M;
                cur->mat = mat1;
                cur->flag = flag1;

                pthread_create(&threads[i], NULL, read_file, (void*)cur);
            }

            int MAX_THREADS2 = MAX_THREADS - MAX_THREADS1;
            for (int i = 0; i < MAX_THREADS2; ++i) {
                thread_params* cur = (thread_params*)(malloc(sizeof(thread_params)));

                cur->offset = offset_2;
                cur->path = argv[5];
                cur->row_from = i * K / MAX_THREADS2;
                cur->row_to = (i + 1) * K / MAX_THREADS2 - 1;
                if (i == MAX_THREADS2 - 1) cur->row_to = K - 1;
                cur->col_cnt = M;
                cur->mat = mat2;
                cur->flag = flag2;

                pthread_create(&threads[i + MAX_THREADS1], NULL, read_file, (void*)cur);
            }
        }

        for (int i = 0; i < MAX_THREADS; ++i)
            pthread_join(threads[i], NULL);
        clock_gettime(CLOCK_REALTIME, &end_read);

        double time_taken = (end_read.tv_sec - start_read.tv_sec) + (end_read.tv_nsec - start_read.tv_nsec) / NANO;
        time_taken *= NANO;

        fprintf(csv_ptr, "%d, %lf\n", MAX_THREADS, time_taken);
        // printf("Time taken for reading using %d threads | %lf nanoseconds\n", MAX_THREADS, time_taken);
    }
    fclose(csv_ptr);

    // Detatching shmids
    shmdt((void*)mat1);
    shmdt((void*)mat2);
    shmdt((void*)flag1);
    shmdt((void*)flag2);

    // Removing shared memory after benchmark is over
    shmctl(mat1_id, IPC_RMID, 0);
    shmctl(mat2_id, IPC_RMID, 0);
    shmctl(flag1_id, IPC_RMID, 0);
    shmctl(flag2_id, IPC_RMID, 0);
}
