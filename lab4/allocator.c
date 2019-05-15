#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "allocator.h"

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
      if(memory[i] == -1) {
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
   //printf("%s\n", token);
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
   } else {
      printf("Invalid option. Options include:\n");
      printf("\trequest: RQ P# #bytes Strategy(F,B,W)\n");
      printf("\trelease: RL P#\n");
      printf("\tcompact: C\n");
      printf("\tsummary: STAT\n");
      printf("\t   quit: quit\n\n");
   }
}

void main(int argc, char *argv[]) {
   int max;
   int quit = 0;
   int i = 0;
   char input[100];
   if(argc < 2) {
      printf("Error: Too few arguments.\nUsage: ./allocator <memory size>\n");
      exit(1);
   } else if (argc > 2) {
      printf("Error: Too many arguments.\nUsage: ./allocator <memory size>\n");
      exit(1);
   }
   MEM_SIZE = atoi(argv[1]);
   int memory[MEM_SIZE];
   initialize(memory, max);
   for (i; i < MEM_SIZE; i++) {
      memory[i] = -1;
   }
   while(!quit) {
      printf("allocator> ");
      if(!fgets(input, 100, stdin)) {
         if (feof(stdin)) {
            printf("exit\n");
            break;
         }
      } else if(!strcmp(input, "quit\n")) {
         exit(-1);
      } else {
         checkInput(input, memory);
      }
   }
}
