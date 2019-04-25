#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

int list[LENGTH];
int result[LENGTH];

int main() {
   int i;
   //pthread_rwlock_t wrlock = PTHREAD_RWLOCK_INITIALIZER;
   //pthread_rwlock_wrlock(&wrlock);
   //createFile();
   //pthread_rwlock_unlock(&wrlock);
   //pthread_rwlock_rdlock(&wrlock);
   readFile();
   //pthread_rwlock_unlock(&wrlock);
   //for (i = 0; i < LENGTH; i++) {
   //   printf("%d\n", list[i]);
   //}
   //createFile();
   //readFile();
   pthread_t workers[THREAD_COUNT];
   par* data = (par*)malloc(sizeof(par));
   data->from = 0;
   data->to = (LENGTH/2) - 1;
   pthread_create(&workers[0],0,sorter,data);
   par* data2 = (par*)malloc(sizeof(par));
   data2->from = (LENGTH/2);
   data2->to = LENGTH-1;
   pthread_create(&workers[1],0,sorter,data2);
   /* Wait for both threads to finish */
   for (i = 0; i < 2; i++) {
      pthread_join(workers[i],NULL);
   }
   par* data3 = (par*)malloc(sizeof(par));
   data3->from = 0;
   data3->to = LENGTH/2;
   pthread_create(&workers[2],0,merger,data3);
   pthread_join(workers[2], NULL);
   printToFile(result);
   return 0;
}

void printToFile(int result[]) {
   FILE *f;
   int i;
   f = fopen("sorted.txt", "w");
   fprintf(f, "%d", result[0]);
   for (i = 1; i < LENGTH; i++) {
      if ((i % 10) == 0) {
         fprintf(f, "\n%d", result[i]);
      } else {
         fprintf(f, " %d", result[i]);
      }
   }
}

void *merger(void *params) {
   par* p = (par*)params;
   int start = p->from;
   int end = p->to+1;
   int i,j,tmp;
   for (i = start; i < end; i++) {
      for (j = start; j < end-i; j++) {
         if (result[j] > result[j+1]) {
            tmp = result[j];
            result[j] = result[j+1];
            result[j+1] = tmp;
         }
      }
   }
   pthread_exit(0);
}

void *sorter(void *params) {
   par* p = (par *)params;
   int start = p->from;
   int end = p->to+1;
   int i,j,tmp;
   for (i = start; i < end; i++) {
      for (j = start; j < end-i-1; j++) {
         if (list[j] > list[j+1]) {
            tmp = list[j];
            list[j] = list[j+1];
            list[j+1] = tmp;
         }
      }
   }
   for (i = start; i < end; i++) {
      result[i] = list[i];
   }
   pthread_exit(0);
}

void readFile() {
   FILE *f;
   int i;
   f = fopen("numbers.txt", "r");
   for (i = 0; i < LENGTH; i++) {
      fscanf(f, " %d", &list[i]);
   }
   close(f);
}

void createFile() {
   FILE *f;
   int i;
   int lower = 1;
   int upper = 10000;
   f = fopen("numbers.txt", "w");
   for (i = 0; i < LENGTH; i++) {
      fprintf(f, "%d ",((rand()%(upper - lower + 1)) + lower));
   }
   close(f);
}
