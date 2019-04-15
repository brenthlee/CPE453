#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"

void add(char *name, int priority, int burst, ListNode** cur) {
   Task* task = (Task*)calloc(1,sizeof(Task));
   task->name = name;
   task->priority = priority;
   task->burst = burst;
   //addTail(cur, task);
   insert(cur, task);
}

void schedule(ListNode* cur) {
//   traverse(*cur);
   ListNode *temp, *head;
   int tmpFlag;
   int flag = 0;
   head = cur;
   temp = head;
   int slice;
   //int chunk = /traverse(head);
   while (!flag) {
      temp = head;
      tmpFlag = 0;
      while (temp != NULL) {
         if (temp->task->burst > 0) {
            //TODO: How to choose the chunk time
            //run(temp->task, temp->task->burst);
            slice = (temp->task->burst < 10) ? temp->task->burst : 10;
            run(temp->task, slice);
            temp->task->burst -= slice;
            tmpFlag = 1;
            if (temp->task->burst <= 0) {
               printf("Task %s finished.\n", temp->task->name);
            }
         }
         temp = temp->next;
      }
      if (!tmpFlag) {
         flag = 1;
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
