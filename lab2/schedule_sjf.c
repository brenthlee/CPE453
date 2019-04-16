#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"

void add(char *name, int priority, int burst, ListNode** cur, int oldTid) {
   Task* task = (Task*)calloc(1,sizeof(Task));
   task->name = name;
   task->priority = priority;
   task->burst = burst;
   task->tid = oldTid;
   addTail(cur, task);
}

void schedule(ListNode* cur) {
//   traverse(*cur);
   ListNode *temp, *head;
   int size = traverse(cur);
   int index, tmpFlag;
   int i;
   int lowBurst, lowIndex;
   int stored[size];
   for (i = 0; i < size; i++) {
      stored[i] = 0;

   }
   i = 0;
   int flag = 0;
   head = cur;
   temp = head;
   while (!flag) {
      temp = head;
      tmpFlag = 0;
      index = 0;
      lowIndex = -1;
      lowBurst = 50000;
      while (temp != NULL) {
         if (stored[index] == 0 && temp->task->burst < lowBurst) {
            lowBurst = temp->task->burst;
            lowIndex = index;
            tmpFlag = 1;
         }
         index++;
         temp = temp->next;
      }
      stored[lowIndex] = 1;
      temp = head;
      for (i = 0; i < lowIndex; i++) {
         temp = temp->next;
      }
      if (!tmpFlag) {
         flag = 1;
      } else {
         run(temp->task, temp->task->burst);
         printf("Task %s finished.\n", temp->task->name);
      }
   }
}

void freeNodes(ListNode** cur) {
   ListNode* cur1 = *cur;
   ListNode* tmp = cur1;
   while (cur1 != NULL) {
      tmp = cur1->next;
      free(cur1->task->name);
      free(cur1->task);
      free(cur1);
      cur1 = tmp;
   }
}
