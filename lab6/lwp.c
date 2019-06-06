#include <stdio.h>
#include <stdlib.h>
#include "lwp.h"
#include "rr.h"
#include "stackStuff.h"
#include "threadStuff.h"

static unsigned long* initStack(lwpfun function, void* arg, unsigned long* stackPointer);

static context saved; //saved context (typedefed for struct threadinfo_st)
static thread curThread = NULL;
static scheduler curSched = &rr_publish;
static int isRunning = 0;

tid_t lwp_create(lwpfun function, void* arg, size_t stackSize) {
   thread newThread = NULL;
   unsigned long* basePointer; //base pointer
   unsigned long* stackPointer; //stack pointer
   if ((newThread = malloc(sizeof(context))) == NULL) {
      return -1;
   }
   if ((basePointer = allocateStack(stackSize, sizeof(unsigned long), (void**)&stackPointer)) == NULL) {
      return -1;
   }
   stackPointer = initStack(function, arg, stackPointer);
   newThread->tid = createThreadId(newThread);
   newThread->stack = basePointer; //create a stack
   newThread->state.fxsave = FPU_INIT;
   newThread->stacksize = (unsigned)stackSize;
   newThread->state.rdi = (unsigned long)arg;
   newThread->state.rsp = (unsigned long)stackPointer;
   newThread->state.rbp = (unsigned long)stackPointer;
   curSched->admit(newThread);
   return (newThread->tid);
}

static unsigned long* initStack(lwpfun function, void* arg, unsigned long* stackPointer) {
   unsigned long* tmpBasePointer;
   stackPointer--;
   *stackPointer = (unsigned long)(lwp_exit);
   stackPointer--;
   *stackPointer = (unsigned long)(function);
   stackPointer--;
   *stackPointer = (unsigned long)(0xDEADBEEF); //used to align. Used from shown code in class
   tmpBasePointer = stackPointer;
   return tmpBasePointer;
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

void lwp_yield(void) {
   thread cur;
   cur = curThread;
   curThread = curSched->next();
   if (curThread == NULL) {
      curThread = &saved;
   }
   swap_rfiles(&(cur->state), &(curThread->state));
}

void lwp_exit(void) {
   unsigned long tmpStackPointer;
   thread nextThread;
   tmpStackPointer = saved.state.rsp;
   tmpStackPointer -= (tmpStackPointer % 16);
   SetSP(tmpStackPointer);
   curSched->remove(curThread);
   nextThread = curSched->next();
   freeStack(curThread->stack);
   free(curThread);
   curThread = nextThread;
   if (!nextThread) {
      isRunning = 0;
      load_context(&saved.state);
   } else {
      load_context(&curThread->state);
   }
}

void lwp_set_scheduler(scheduler function) {
   scheduler oldSched = curSched;
   thread th;
   if (function != curSched) {
      if (!function) {
         curSched = roundRobin;
      } else {
         curSched = function;
      }
      if (curSched->init != NULL) {
         curSched->init();
      }
      while ((th = oldSched->next())) {
         oldSched->remove(th);
         curSched->admit(th);
      }
      if (oldSched->shutdown != NULL) {
         oldSched->shutdown();
      }
   }
}

void lwp_start(void) {
   if (isRunning) {
      printf("Threads are already active\n");
   } else {
      curThread = curSched->next();
      if (curThread) {
         isRunning = 1;
         swap_rfiles(&(&saved)->state, &curThread->state);
      }
   }
}

void lwp_stop(void) {
   if (!isRunning) {
      printf("There are no active threads\n");
   } else {
      isRunning = 0;
      swap_rfiles(&curThread->state, &(&saved)->state);
   }
}
