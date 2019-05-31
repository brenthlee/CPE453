#include <stdio.h>
#include <stdlib.h>
#include "lwp.h"
#include "schedule.h"

static tid_t threadId = 0;
thread listOfThreads = NULL;
static thread curThread = NULL;
static scheduler curSched = NULL;

tid_t lwp_create(lwpfun function, void* arg, size_t stackSize) {
   thread newThread = NULL;
   unsigned long* basePointer; //base pointer
   unsigned long* stackPointer; //stack pointer
   newThread = malloc(sizeof(context)); //context is typedefed for struct threadinfo_st
   newThread->stack = malloc(sizeof(unsigned long) * stackSize); //create a stack
   newThread->stacksize = stackSize;
   newThread->tid = (++threadId);
   newThread->state.rdi = (unsigned long)arg;
   newThread->state.fxsave = FPU_INIT;
   stackPointer = newThread->stack + stackSize;
   stackPointer--;
   *stackPointer = (unsigned long)lwp_exit; //return address
   stackPointer--;
   *stackPointer = (unsigned long)function; //function process address
   stackPointer--;
   //*stackPointer = 0xAAAABBBB; //might need this for no segfault //garbage
   basePointer = stackPointer;
   newThread->state.rsp = (unsigned long)stackPointer;
   newThread->state.rbp = (unsigned long)basePointer;
   if (!curSched) {
      curSched = roundRobin;
   }
   curSched->admit(newThread);
   if (!listOfThreads) {
      listOfThreads = newThread;
   } else {
      thread cur = listOfThreads;
      while (cur->lib_one) {
         cur = cur->lib_one;
      }
      cur->lib_one = newThread;
      newThread->lib_two = cur;
   }
   return threadId;
}

tid_t lwp_gettid(void) {
   if (curThread != NULL) {
      return curThread->tid;
   }
   return NO_THREAD;
}

scheduler lwp_get_scheduler(void) {
   return curSched;
}

thread tid2thread(tid_t threadId) {
   thread cur = listOfThreads;
   while (cur && (cur->tid != threadId)) {
      cur = cur->lib_one;
   }
   return cur;
}

void  lwp_exit(void) {
}

void  lwp_yield(void) {
}

void  lwp_start(void) {
}

void  lwp_stop(void) {
}

void  lwp_set_scheduler(scheduler fun) {
}
