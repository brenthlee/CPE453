#ifndef SAFESTACK_H
   #define SAFESTACK_H

   #include <sys/types.h>
   
   void* allocateStack(size_t stackSize, size_t tmpSize, void** stackPointer);
   void freeStack(void* basePointer);
   
   typedef struct safeStack {
      void* basePointer;
      void* page;
      struct safeStack* next;
   } safeStack;

#endif
