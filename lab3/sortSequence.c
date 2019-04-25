#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 2
#define ARR_SIZE 10000

void *sorter(void *params);
void *merger(int *params);
int comparator(int* a, int* b);

typedef struct task {
    int start;
    int end;
} task;

struct timespec start, finish;
double elapsed;

int arr[ARR_SIZE];

int main() {
    int i, j, k;
    FILE *f;
    task tskList[NUM_THREADS];
    task tsk;
    pthread_t th1;
    f = fopen("numbers.txt", "r");
    for(i = 0; i < ARR_SIZE; i++) {
        fscanf(f," %d",&arr[i]);
    }
    close(f);
    tsk.start = 0;
    tsk.end = ARR_SIZE;
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_create(&th1, NULL, sorter, &tsk);
    pthread_join(th1, NULL);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("time elapsed sequence: %f\n", elapsed);
    f = fopen("sorted2.txt", "w");
    fprintf(f, "%d", arr[0]);
    for (i = 1; i < ARR_SIZE; i++) {
        if (i % 10 == 0) {
            fprintf(f, "\n%d", arr[i]);
        } else  {
            fprintf(f, " %d", arr[i]);
        }
    }
    fprintf(f,"\n");
    close(f);
}

void *sorter(void *arg) {
    task *tsk = arg;
    qsort((void*)&arr[tsk->start], (tsk->end - tsk->start), sizeof(int), 
         (int (*)(const void *, const void *)) comparator); 
}

int comparator(int *a, int *b) {
    if (*a < *b) return -1;
    else if (*a == *b) return 0;
    return 1;
}
