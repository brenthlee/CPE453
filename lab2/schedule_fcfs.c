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

void schedule(ListNode** cur) {
   traverse(*cur);
}
