#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

thread myList = NULL;
thread toExec = NULL;
static scheduler mySchedule = {NULL, NULL, admt, rmv, nxt};
scheduler roundRobin = &mySchedule;

void admt(thread newThread) {
   thread tmp = myList;
   if (!myList) {
      myList = newThread;
      myList->sched_one = NULL;
      myList->sched_two = NULL;
   } else {
      while (!temp->sched->one) {
         temp = temp->sched_one;
      }
      newThread->sched_one = NULL;
      newThread->sched_two = tmp;
      tmp->sched->one = newThread;
   }
}

void rmv(thread delThread) {
   thread tmp = myList;
   if (!(tmp && delThread)) {
   } else {
      if (tmp->tid == delThread->tid) {
         myList = delThread->sched_one;
      }
      if (delThread->sched_one) {
         delThread->sched_one->sched_two = delThread->sched_two;
      }
      if (delThread->sched_two) {
         delThread->sched_two->sched_one = delThread->sched_one;
      }
   }
}

thread nxt() {
   if (!toExec) {
      toExec = myList;
   } else {
      toExec = toExec->sched_one;
      if (!toExec) {
         toExec = myList;
      }
   }
   return toExec;
}
