#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 2
#define ARR_SIZE 10000

void *sorter(void *params);
void *merger(int *params);
void populate(int arr[]);
int comparator(int* a, int* b);

typedef struct task {
    int start;
    int end;
} task;

int arr[ARR_SIZE];
int result[ARR_SIZE];

int main() {
    struct timespec start, finish;
    double elapsed;
    int i, j, k;
    FILE *f;
    task tskList[NUM_THREADS];
    task *tsk;
    pthread_attr_t attr;
    pthread_t workers[NUM_THREADS];
    f = fopen("numbers.txt", "r");
    for(i = 0; i < ARR_SIZE; i++) {
        fscanf(f," %d",&arr[i]);
    }
    close(f);

    for (i = 0; i < NUM_THREADS; i++) {
        tsk = &tskList[i];
        tsk->start = i * (ARR_SIZE / 2);
        tsk->end   = (i + 1) * (ARR_SIZE / 2);
    }
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < NUM_THREADS; i++) {
        tsk = &tskList[i];
        pthread_create(&workers[i], NULL, sorter, tsk); 
    }
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("elapsed time threaded: %f\n", elapsed);
    i = tskList[0].start;
    j = tskList[1].start;
    k = 0;
    while (i < tskList[0].end && j < tskList[1].end) {
        if (arr[i] < arr[j]) {
            result[k] = arr[i++];
        } else {
            result[k] = arr[j++];
        }
        k++;
    }
    while (i < tskList[0].end) {
        result[k++] = arr[i++];
    }
    while (j < tskList[1].end) {
        result[k++] = arr[j++];
    }
    f = fopen("sorted.txt", "w");
    fprintf(f, "%d", result[0]);
    for (i = 1; i < ARR_SIZE; i++) {
        if (i % 10 == 0) {
            fprintf(f, "\n%d", result[i]);
        } else  {
            fprintf(f, " %d", result[i]);
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
