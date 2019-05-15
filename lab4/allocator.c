#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "allocator.h"

void worstFit(int* memory, int pid, char* size) {
   int i = 0;
   int j = 0;
   int prev = -1;
   int pSize = atoi(size);
   // static (update occasionally)
   int currMax = 0;
   int maxStart = 0;
   int count = 0;
   int start = 0;
   int startFlag = 0;
   int endFlag;

   for(i = 0; i < MEM_SIZE; i++) { 
      //printf("memory %d: memory loc: %d\n", i, memory[i]);
      if(memory[i] == -1) {
         if(startFlag == 0) {  
            start = i;
            startFlag = 1;
         }
         count++;
         //printf("count: %d, pSize: %d\n", count, pSize);
         if(count > currMax) {
            currMax=count;
            maxStart = start;
            //printf("currMax: %d, maxStart: %d\n", currMax, maxStart);
         }
      } else {
         count = 0;
         startFlag = 0; //wait for empty space
         currMax = 0;
      }
   }

   if(currMax < pSize) {
      printf("Not enough space\n");
   } else {
      //printf("Able to write ");
      for(j = start; j < pSize + start; j++) {
         memory[j] = pid;
      }
   }
   //printf("filled: %d from start: %d\n", pSize, start);
}

// first fit algorithm
void firstFit(int* memory, int pid, char* size) {
   int i = 0;
   int j = 0;
   int prev = -1;
   int pSize = atoi(size);
   int count = 0;
   int start = 0;
   int startFlag = 0;
   int endFlag;
   for(i = 0; i < MEM_SIZE; i++) { //should probably error check atoi...
      if(memory[i] == -1) {
         if(startFlag == 0) {
            start = i;
            startFlag = 1;
         }
         count++;
         if (count == pSize) {
            break;
         }
      } else {
         startFlag = 0;
         count = 0;
      }
   }

   if(count < pSize) {
      printf("Error: %d bytes available\n", count);
      printf("       %d bytes requested\n", pSize);
   } else {
      for (i = start; i < (start+count); i++) {
         memory[i] = pid;
      }
   }
}

// begins request process
void request(int* memory, int pid, char* size, char* type){
   //printf("make request: %s %s %s\n", process, size, type);
   if(!strcmp(type, "F\n")) {
      firstFit(memory, pid, size);
   } else if (!strcmp(type, "W\n")) {
      worstFit(memory, pid, size);
   } else {
      printf("screw you\n");
   }
}

// begins release process
void release(int* memory, int pid) {
   int i;
   int changed = 0;
   for (i = 0; i < MEM_SIZE; i++) {
      if (memory[i] == pid) {
         memory[i] = -1;
         changed = 1;
      }
   }
   if (!changed) {
      printf("Error: Process P%d does not exist\n", pid);
   }
}

void printIt(int* memory) {
   int i;
   for(i = 0; i < MEM_SIZE; i++) {
      printf("| %d |", memory[i]);
   }
   printf("\n");
}

void swap(int* a, int index1, int index2) {
   int temp = a[index2];
   a[index2] = a[index1];
   a[index1] = temp;
}

// begins compaction process
void compact(int* memory) {
   int i;
   int j;
   int pIndex = 0;
   int processes[MEM_SIZE];
   int procIndex = 0;
   int process = -1;
   int pSize = 0;

   for(i = 0; i < MEM_SIZE; i++) {
      if(memory[i] == -1) {
         for(j = i; j < MEM_SIZE; j++) {
            if(memory[j] != -1) {
               swap(memory, i++, j);
            }
         }
      }
   }
   printIt(memory);
//   for (i = 0; i < MEM_SIZE; i++) {
//      if(memory[i] != -1) {
//         processes[j] = memory[i];
//      }
//   }
//
//   for(i = 0; i < MEM_SIZE; i++) {
//      memory[i] = processes[i];
//   }
//   for (i = 0; i < MEM_SIZE; i++) {
//      if (memory[i] == process) {
//         pSize++;
//      }
//   }
}

// begins report process
void report(int* memory){
   int process = -1;
   int start = 0;
   int sFlag = 0;
   int i;
   printIt(memory);
   for (i = 0; i < MEM_SIZE; i++) {
      if (memory[i] != process) {
         if (sFlag == 0) {
            process = memory[i];
         } else {
            if (i == (MEM_SIZE - 1)) {
               if (memory[i-1] == -1) {
                  printf("Addresses [%d:%d] Unused\n",start,i-1);
                  printf("Addresses [%d:%d] Process P%d\n",i,i,memory[i]);
               } else {
                  printf("Addresses [%d:%d] Process P%d\n",start,i-1,memory[i-1]);
                  if (memory[i] == -1) {
                     printf("Addresses [%d:%d] Unused\n",i,i);
                  } else {
                     printf("Addresses [%d:%d] Process P%d\n",i,i,memory[i]);
                  }
               }
            } else {
               if (memory[i-1] == -1) {
                  printf("Addresses [%d:%d] Unused\n",start,i-1);
               } else {
                  printf("Addresses [%d:%d] Process P%d\n",start,i-1,process);
               }
               start = i;
               process = memory[i];
            }
         }
      } else if (i == (MEM_SIZE - 1)) {
         if (memory[i] == -1) {
            printf("Addresses [%d:%d] Unused\n",start,i);
         } else {
            printf("Addresses [%d:%d] Process P%d\n",start,i,memory[i]);
         }
      }
      sFlag = 1;
   }
}

void checkInput(char* input, int* memory) {
   char* token;
   char* process;
   char* size;
   char* type;
   int i = 0;
   int pid;
   int length = strlen(input);

   token = strtok(input, " ");
   //printf("%s\n", token);
   if(!strcmp("RQ", token)) {
      process = strtok(NULL, " ");
      size = strtok(NULL, " ");
      type = strtok(NULL, " ");
      pid = atoi(&process[1]);
      request(memory, pid, size, type);
   } else if(!strcmp("RL", token)) {
      process = strtok(NULL, " ");
      pid = atoi(&process[1]);
      release(memory, pid);
   } else if(!strcmp("C\n", token)) {
      compact(memory);
   } else if(!strcmp("STAT\n", token)) {
      report(memory);
   } else {
      printf("Invalid option. Options include:\n");
      printf("\trequest: RQ P# #bytes Strategy(F,B,W)\n");
      printf("\trelease: RL P#\n");
      printf("\tcompact: C\n");
      printf("\tsummary: STAT\n");
      printf("\t   quit: X\n\n");
   }
}

void main(int argc, char *argv[]) {
   int max;
   int quit = 0;
   int i;
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
   for (i = 0; i < MEM_SIZE; i++) {
      memory[i] = -1;
   }
   while(!quit) {
      printf("allocator> ");
      if(!fgets(input, 100, stdin)) {
         if (feof(stdin)) {
            printf("exit\n");
            break;
         }
      } else if(!strcmp(input, "X\n")) {
         exit(-1);
      } else {
         checkInput(input, memory);
      }
   }
}
