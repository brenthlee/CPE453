#ifndef THREADSTUFF_H
   #define THREADSTUFF_H

   #include "lwp.h"

   tid_t create_tid(thread lwp);
   thread tid2thread(tid_t threadId);
   int tid2nonce(tid_t threadId);

#endif
