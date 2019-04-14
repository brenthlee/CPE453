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
   int tmpFlag;
   int flag = 0;
   head = cur;
   temp = head;
   //int chunk = /traverse(head);
   while (!flag) {
      temp = head;
      tmpFlag = 0;
      while (temp != NULL) {
         if (temp->task->burst != 0) {
            //run(temp->task, temp->task->burst);
            run(temp->task, 10);
            temp->task->burst -= 10;
            tmpFlag = 1;
            if (temp->task->burst == 0) {
               printf("Task %s finished.\n", temp->task->name);
            }
         }
         temp = temp->next;
      }
      if (!tmpFlag) {
         flag = 1;
      }
   }
   while (temp != NULL) {
      printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
      //run(temp->task, temp->task->burst);
      temp = temp->next;
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
