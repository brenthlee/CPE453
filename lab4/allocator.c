//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//
//#define ARR_SIZE 1000
//#define TRUE 1
//#define FALSE 0
//
//typedef struct process {
//   int start;
//   int end;
//   char* name;
//} process;
#include "allocate.h"
// first fit algorithm
void firstFit(int* memory, char* process, char* size) {
   int i = 0;
   int prev = -1;
   int pSize = atoi(size);
   // static (update occasionally)
   int availSize = 0;
   int index = 0;
   // dynamic
   int currSize;

   for(i = 0; i < MEM_SIZE; i++) { //should probably error check atoi...
      if(memory[i] == -1) {
         if(flag == 0) {
            start = i;
         }
         count++;
         if(count == pSize) {
            // do stuff
         }

      }
////        if(memory[i] != -1 || (MEM_SIZE -1) == i) {               // if curr is not free
////           if(prev == -1) {                 // if previous WAS free
////              if(currSize >= atoi(size)) {  // if P will fit
////                 availSize = currSize;
////                 currSize = 0;
////              }
////           }
////        } else {
////           currSize++;
////           if(prev != -1) {
////              index = i;
////           }
////        }
////        prev = memory[i];
//      if(prev == -1 && memory[i] != -1) { //if previous was free
//         index = i;
//      } 
//      if(memory[i] == -1) {
//         currSize++;
//      } else if (prev != -1 && memory[i] == -1){
//         // check if process will fit in current block
//         if(currSize >= atoi(size)) {
//            // update available size
//            availSize = currSize;
//         }
//      }
//      prev = memory[i];
   }
   printf("available: %d, start: %d\n", availSize, index);
}

// begins request process
void request(int* memory, char* process, char* size, char* type){
   printf("make request: %s %s %s\n", process, size, type);
   if(!strcmp(type, "F\n")) {
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

void initialize(int* memory, int size) {
   int i = 0;
   for (i; i < size; i++) {
      memory[i] = -1;
   }
}

void checkInput(char* input, int* memory) {
   char* token;
   char* process;
   char* size;
   char* type;
   int i = 0;
   int length = strlen(input);

   token = strtok(input, " ");
   printf("%s\n", token);
   if(!strcmp("RQ", token)) {
      process = strtok(NULL, " ");
      printf("%s\n", process);
      size = strtok(NULL, " ");
      type = strtok(NULL, " ");
      request(memory, process, size, type);
   } else if(!strcmp("RL", token)) {
      process = strtok(NULL, " ");
      release(memory, process);
   } else if(!strcmp("C", token)) {
      compact(memory);
   } else if(!strcmp("STAT", token)) {
      report(memory);
   }
}

void main(int argc, char *argv[]) {
   int max;
   int quit = FALSE;
   char input[100];
   if(argc == 1) {
      printf("Usage: ./allocator <memory size>\n");
      exit(1);
   }
   MEM_SIZE = atoi(argv[1]);
   int memory[MEM_SIZE];
   initialize(memory, max);
   
   while(quit == FALSE) {
      printf("allocator> ");
      if(!fgets(input, 100, stdin)) {
         if (feof(stdin)) {
            printf("exit\n");
            break;
         }
      } else if(!strcmp(input, "quit\n")) {
         quit = TRUE;
      } else {
         checkInput(input, memory);
      }
   }
}
