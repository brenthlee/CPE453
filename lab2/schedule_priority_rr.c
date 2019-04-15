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
   addTail(cur, task);
}

void schedule(ListNode* cur) {
//   traverse(*cur);
   ListNode *temp, *head;
   int size = traverse(cur);
   int index, tmpFlag, slice;
   int i, rcount = 0, lowPriority, lowIndex;
   int stored[size];
   int rrflag, tmprrFlag;
   Task* rr[size];
   int flag = 0;
   for (i = 0; i < size; i++) {
      stored[i] = 0;
   }
   i = 0;

   head = cur;
   temp = head;
   //int chunk = /traverse(head);
   while (!flag) {
      temp = head;
      tmpFlag = 0;
      index = 0;
      
      lowIndex = -1;
      lowPriority = 50000;
      while (temp != NULL) { //while this is not empty
         if (stored[index] == 0 && temp->task->priority < lowPriority) {
            lowPriority = temp->task->priority;
            lowIndex = index;
            tmpFlag = 1;
         }
         index++;
         temp = temp->next;
      }
      stored[lowIndex] = 1;
      temp = head;
      // already found location of lowest priority
      rcount = 0;
      for (i = 0; i < size; i++) { 
         // making list of equal priority tasks (rr)
         if (temp->task->priority == lowPriority) {
            rr[rcount] = temp->task;
            rcount++;
            stored[index] = 1;
         }
         temp = temp->next;
      }
      if (!tmpFlag) {
         flag = 1;
      } else if (rcount > 1) {
         // if > 1 with this priority
         rrflag = 0;
         while(!rrflag) {
           tmprrFlag = 0;    
           for(i = 0; i < rcount; i++) {
              if(rr[i]->burst > 0) {
                 slice = (rr[i]->burst < 10) ? rr[i]->burst : 10;
                 run(rr[i], slice);
                 rr[i]->burst -= slice;
                 tmprrFlag = 1;
                 if (rr[i]->burst <= 0) {
                    printf("Task %s finished\n", rr[i]->name);
                 }
              }
         }
           if (tmprrFlag == 0) {
              rrflag = 1;
           }
         }
      } else {
         // if only one with this priority
         temp = head;
         for (i = 0; i < lowIndex; i++) { 
         // making list of equal priority tasks (rr)
            temp = temp->next;
         }
         //printf("no rr: %s", temp->task->name);
         run(temp->task, temp->task->burst);
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
