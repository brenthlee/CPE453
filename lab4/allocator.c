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
   int j = 0;
   int prev = -1;
   int pSize = atoi(size);
   int pid = atoi(&process[1]);
   printf("%d\n", pid);
   // static (update occasionally)
   int count = 0;
   int start = 0;
   int startFlag = 0;
   int endFlag;
   // dynamic
   int currSize;

   for(i = 0; i < MEM_SIZE; i++) { //should probably error check atoi...
      printf("memory %d: memory loc: %d\n", i, memory[i]);
      if(memory[i] == -1) {      // 
         if(startFlag == 0) {
            start = i;
            startFlag = 1;
         }
         count++;
         printf("count: %d, pSize: %d\n", count, pSize);
         if(count == pSize) {
            printf("count when writing: %d, start: %d\n", count, start);
            for(j = start; j < count; j++) {
               memory[j] = pid;
            }
            break;
         }
      } else {
         count = 0;
      }
   }

   printf("available: %d, start: %d\n", count, start);
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
   int i = 0;
   char input[100];
   if(argc == 1) {
      printf("Usage: ./allocator <memory size>\n");
      exit(1);
   }
   MEM_SIZE = atoi(argv[1]);
   int memory[MEM_SIZE];
   initialize(memory, max);
   for (i; i < MEM_SIZE; i++) {
      memory[i] = -1;
   }
   printf("memory: %d\n", memory[0]);
   
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
