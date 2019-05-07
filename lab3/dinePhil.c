#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

#define NUM_PHIL 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (pnum+4) % NUM_PHIL
#define RIGHT (pnum+1) % NUM_PHIL

void test(int pnum);
void takeFork(int pnum);
void putDownFork(int pnum);
void* philosopher(void* pnum);

int state[NUM_PHIL];
int phils[NUM_PHIL];

sem_t mutex;
sem_t sems[NUM_PHIL];

int main() {
   int i;
   pthread_t threads[NUM_PHIL];
   sem_init(&mutex, 0, 1);
   for (i = 0; i < NUM_PHIL; i++) {
      phils[i] = i;
   }
   for (i = 0; i < NUM_PHIL; i++) {
      sem_init(&sems[i],0,0);
   }
   for (i = 0; i < NUM_PHIL; i++) {
      pthread_create(&threads[i], NULL, philosopher, &phils[i]);
      printf("Philosopher %d is thinking\n", i+1);
   }
   for (i = 0; i < NUM_PHIL; i++) {
      pthread_join(threads[i], NULL);
   }
   printf("ALL OF THE PHILOSOPHERS HAVE EATEN\n");
}

void* philosopher(void* num) {
   int* i = num;
   sleep(1);
   takeFork(*i);
   srand(5);
   sleep((rand()%4)+2);
   putDownFork(*i);
}

void putDownFork(int pnum) {
   sem_wait(&mutex);
   state[pnum] = THINKING;
   printf("Philosopher %d is putting down forks %d and %d\n",
           pnum+1, LEFT+1, pnum+1);
   printf("Philosopher %d is now thinking\n", pnum+1);
   test(LEFT);
   test(RIGHT);
   sem_post(&mutex);
}

void takeFork(int pnum) {
   sem_wait(&mutex);
   state[pnum] = HUNGRY;
   printf("Philosopher %d is hungry\n", pnum+1);
   test(pnum); //eat if neighbors are not eating
   sem_post(&mutex);
   sem_wait(&sems[pnum]); //wait for signal to allow eating
   sleep(1);
}

void test(int pnum) {
   if (state[pnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
      state[pnum] = EATING;
      printf("Philosopher %d took fork %d and %d\n", pnum+1, LEFT+1, pnum+1);
      printf("Philosopher %d is now eating\n", pnum+1);
      sem_post(&sems[pnum]);
   }
}
