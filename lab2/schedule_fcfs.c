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
   ListNode *cur1 = cur;
   while (cur1 != NULL) {
      run(cur1->task, cur1->task->burst);
      printf("Task %s finished.\n", cur1->task->name);
      cur1 = cur1->next;
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
