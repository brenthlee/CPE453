#include <stdlib.h>
#include <stdio.h>
#include "lwp.h"
#include "rr.h"

static void rr_admit(thread newThread);
static void rr_remove(thread delThread);
static thread rr_next();
static thread head = NULL;
static int next = 0;
struct scheduler rr_publish = {NULL, NULL, rr_admit, rr_remove, rr_next};
scheduler roundRobin = &rr_publish;

static void rr_admit(thread newThread) {
   if (head != NULL) {
      newThread->sched_one = head;
      newThread->sched_two = head->sched_two;
      newThread->sched_two->sched_one = newThread;
      head->sched_two = newThread;
   } else {
      next = 0;
      head = newThread;
      head->sched_one = newThread;
      head->sched_two = newThread;
   }
}

static void rr_remove(thread delThread) {
   delThread->sched_two->sched_one = delThread->sched_one;
   delThread->sched_one->sched_two = delThread->sched_two;
   if (delThread == head) {
      if (delThread->sched_one != delThread) {
         head = delThread->sched_two;
      } else {
         head = NULL;
      }
   }
}

static thread rr_next() {
   if (head) {
      if (next) {
         head = head->sched_one;
      } else {
         next = 1;
      }
   }
   return head;
}
