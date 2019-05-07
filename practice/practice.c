#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10

int SUM = 0;
void* runner(void* para);
pthread_mutex_t sumlock;

typedef struct args {
   int lower;
   int upper;
} arg;

int main() {
   pthread_t tid;
   int i;
   int number = 1000;
   arg argList[NUM_THREADS];
   arg* cur;
   pthread_t workers[NUM_THREADS];
   int chunk = number / NUM_THREADS;
   for (i = 0; i < NUM_THREADS; i++) {
      cur = &argList[i];
      cur->lower = i*chunk+1;
      cur->upper = (i*chunk)+chunk;
      pthread_create(&workers[i], NULL, runner, cur);
   }
   for (i = 0; i < NUM_THREADS; i++) {
      pthread_join(workers[i], NULL);
   }
   printf("%d\n", SUM);
}

void* runner(void* param) {
   int i;
   arg* cur = (arg*)param;
   int upper = cur->upper;
   int lower = cur->lower;
   int sum = 0;
   for (i = lower; i <= upper; i++) {
      sum += i;
   }
   printf("partial sum: %d\n", sum);
   //pthread_mutex_lock(&sumlock);
   SUM += sum;
   //pthread_mutex_unlock(&sumlock);
   pthread_exit(0);
}
