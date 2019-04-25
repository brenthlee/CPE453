#ifndef SORT_H
   #define SORT_H

   #define THREAD_COUNT 3
   #define LENGTH 10000
   typedef struct {
      int from;   //from index
      int to;     //to index
   } par;
   void *sorter(void *params);
   void *merger(void *params);
   void createFile();
   void readFile();
   void printToFile(int list[]);

#endif
