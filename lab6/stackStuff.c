#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdint.h>
#include "lwp.h"
#include "stackStuff.h"
#include "threadStuff.h"

static char tmpStack[(1<<16)];
static jmp_buf bailbuf;
static safeStack* listStacks = NULL;
static void addToList(void* basePointer, void* page);
void* removeFromList(void* basePointer);
static void createHandlers();

void segFault(int sig, siginfo_t* info, void* other) {
   int protected = 0;
   safeStack* curStack;
   curStack = listStacks;
   while (curStack && !protected) {
      if (curStack->page <= (void*)(info->si_addr) && (void*)(info->si_addr) < curStack->page + 4096) {
         protected = 1;
      }
      curStack = curStack->next;
   }
   if (protected) {
      siglongjmp(bailbuf, 42);
   } else {
      abort();
   }
}

void* allocateStack(size_t stackSize, size_t tmpSize, void** stackPointer) {
   void* basePointer;
   void* page;
   size_t padding = 4096 * 2;
   int protected;
   basePointer = malloc(stackSize * tmpSize + padding);
   if (basePointer) {
      if ((uintptr_t)basePointer % 4096) {
         page = basePointer + 4096 - (uintptr_t)basePointer % 4096;
      } else {
         page = basePointer;
      }
      if (stackPointer) {
         *stackPointer = page + 4096 + stackSize * tmpSize;
      }
      protected = mprotect(page, 4096, PROT_NONE);
      if (protected == -1) {
         page = NULL;
      }
      addToList(basePointer, page);
   }
   return basePointer;
}

void freeStack(void* basePointer) {
   void* page;
   page = removeFromList(basePointer);
   if (page != NULL) {
      mprotect(page, 4096, PROT_READ|PROT_WRITE);
   }
   free(basePointer);
}

static void addToList(void* basePointer, void* page) {
   safeStack* cur;
   if (page) {
      if (!listStacks) {
         createHandlers();
      }
      if ((cur = malloc(sizeof(safeStack)))) {
         cur->page = page;
         cur->basePointer = basePointer;
         cur->next = listStacks;
         listStacks = cur;
      }
   }
}

void* removeFromList(void* basePointer) {
   safeStack tmp;
   safeStack* curStack;
   safeStack* remStack;
   void* retVal = NULL;
   tmp.next = listStacks;
   curStack = &tmp;
   while (curStack->next && curStack->next->basePointer != basePointer) {
      curStack = curStack->next;
   }
   if (curStack->next) {
      remStack = curStack->next;
      retVal = curStack->next->page;
      curStack->next = curStack->next->next;
      free(remStack);
   }
   return retVal;
}

void overflow(int sig) {
   abort();
}

static void createHandlers() {
   struct sigaction sa;
   stack_t ss;
   ss.ss_sp = tmpStack;
   ss.ss_flags = 0;
   ss.ss_size = (1<<16);
   if (sigaltstack(&ss, NULL)) {
      perror("sigaltstack");
      exit(EXIT_FAILURE);
   }
   sa.sa_handler = overflow;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_ONSTACK;
   if (sigaction(BAILSIGNAL, &sa, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
   }
   sa.sa_sigaction = segFault;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_ONSTACK | SA_SIGINFO;
   if (sigaction(SIGSEGV, &sa, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
   }
   #ifdef __APPLE__
      sa.sa_sigaction = segFault;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = SA_ONSTACK | SA_SIGINFO;
      if (sigaction(SIGBUS, &sa, NULL) == -1) {
         perror("sigaction");
         exit(EXIT_FAILURE);
      }
   #endif
   if (sigsetjmp(bailbuf, 1) == 42) {
      lwp_exit();
   }
}
