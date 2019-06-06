#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>
#include "lwp.h"
#include "threadStuff.h"

static tid_t readThreadId(thread lwp);
typedef void (*sigfun)(int);

#define NONCEMASK 0x1fffff
#define NONCESHIFT 47
#define THREADMASK (~(NONCEMASK << NONCESHIFT))

tid_t createThreadId(thread lwp) {
   static int nonce;
   nonce++;
   if ((long)lwp & !THREADMASK) {
      fprintf(stderr, "Pointer is too big\n");
      exit(EXIT_FAILURE);
   }
   return (((nonce & NONCEMASK) << NONCESHIFT) | (tid_t)lwp);
}

int tid2nonce(tid_t threadId) {
   return (threadId >> NONCESHIFT) & NONCEMASK;
}

thread tid2thread(tid_t threadId) {
   thread lwp;
   tid_t saved;
   lwp = (thread)(threadId & THREADMASK);
   if (lwp != NULL) {
      saved = readThreadId(lwp);
      if (saved != threadId) {
         lwp = NULL;
      }
   }
   return lwp;
}

static jmp_buf loc;
static void catchSegFault(int sig) {
   siglongjmp(loc, 42);
}

static tid_t readThreadId(thread lwp) {
   struct sigaction sa;
   struct sigaction oldSig;
   tid_t retVal;
   sa.sa_handler = (sigfun)catchSegFault;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   if (sigaction(SIGSEGV, &sa, &oldSig) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
   }
   if (sigsetjmp(loc, 1) == 42) {
      retVal = NO_THREAD;
   } else {
      retVal = lwp->tid;
   }
   if (sigaction(SIGSEGV, &oldSig, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
   }
   return retVal;
}
