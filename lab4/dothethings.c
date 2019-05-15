#include "allocate.h"

// first fit algorithm
void firstFit(int* memory, char* process, char* size) {
   int i = 0;
   for(i = 0; i < MEM_SIZE; i++) { //should probably error check atoi...
      
   }
}

// begins request process
void request(int* memory, char* process, char* size, char* type){
   printf("make request: %s %s %s\n", process, size, type);
   if(!strcmp(type, "F")) {
      firstFit(memory, process, size);
   } else {
      printf("screw you\n");
   }
}

// begins release process
void release(int* memory, char* input){
   printf("release: %s\n", input);
}

// begins compaction process
void compact(int* memory){
   printf("compact\n");
}

// begins report process
void report(int* memory){
   printf("report\n");
}
