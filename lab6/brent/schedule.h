#ifndef SCHEDULE_H
   #define SCHEDULE_H

   #include "lwp.h"
   void nxt();
   void rmv(thread delThread);
   void admt(thread newThread);

   thread myList;
   thread toExec;
   scheduler roundRobin;

#endif
