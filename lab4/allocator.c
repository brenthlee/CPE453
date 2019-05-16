#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "allocator.h"

void worstFit(int* memory, int pid, char* size) {
   int i = 0;
   int pSize = atoi(size);
   int currMax = 0;
   int count = 0;
   int start = 0;
   for(i = 0; i < MEM_SIZE; i++) {
      if (memory[i] == -1) {
         count++;
         if (i == MEM_SIZE - 1) {
            if (count > currMax) {
               currMax = count;
               start = i-count+1;
            }
         }
      } else {
         if (count > currMax) {
            currMax = count;
            start = i-count;
         }
         count = 0;
      }
   }
   if(currMax < pSize) {
      printf("Error: %d contiguous bytes available\n", count);
      printf("       %d bytes requested\n", pSize);
   } else {
      for(i = start; i < pSize + start; i++) {
         memory[i] = pid;
      }
   }
}

void quickSort(int arr[], int low, int high, int starts[]) {
   int i,j,pivot,tmp,tmp2;
   if (low < high) {
      pivot = low;
      i = low;
      j = high;
      while (i < j) {
         while (arr[i] <= arr[pivot] && i < high) {
            i++;
         }
         while (arr[j] > arr[pivot]) {
            j--;
         }
         if (i < j) {
            tmp = arr[i];
            tmp2 = starts[i];
            arr[i] = arr[j];
            starts[i] = starts[j];
            arr[j] = tmp;
            starts[j] = tmp2;
         }
      }
      tmp = arr[pivot];
      tmp2 = starts[pivot];
      arr[pivot] = arr[j];
      starts[pivot] = starts[j];
      arr[j] = tmp;
      starts[j] = tmp2;
      quickSort(arr,low,j-1,starts);
      quickSort(arr,j+1,high,starts);
   }
}

void bestFit(int* memory, int pid, char* size) {
   int i = 0;
   int pSize = atoi(size);
   int currMin = INT_MAX;
   int b;
   int firstZFlag = 0;
   int firstNFlag = 0;
   int count = 0;
   int start = 0;
   int mins[MEM_SIZE];
   int starts[MEM_SIZE];
   int minIndex = 0;
   int flag = 0;
   for(i = 0; i < MEM_SIZE; i++) {
      if (memory[i] == -1) {
         if (firstZFlag == 0) {
            firstZFlag = 1;
            starts[minIndex] = i;
         }
         firstNFlag = 0;
         count++;
         if (i == MEM_SIZE-1) {
            mins[minIndex] = count;
            minIndex++;
            if (count < currMin) {
               currMin = count;
            }
         }
      } else {
         if (firstNFlag == 0) {
            mins[minIndex] = count;
            minIndex++;
            if (count < currMin) {
               currMin = count;
            }
         }
         firstZFlag = 0;
         count = 0;
      }
   }
   quickSort(mins, 0, minIndex, starts);
   for (i = 0; i < minIndex+1; i++) {
      if (mins[i] >= pSize) {
         for (b = starts[i]; b < (starts[i] + pSize); b++) {
            memory[b] = pid;
         }
         flag = 1;
         break;
      }
   }
   if(!flag) {
      printf("Error: %d contiguous bytes available\n", count);
      printf("       %d bytes requested\n", pSize);
   }
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
   for(i = 0; i < MEM_SIZE; i++) {
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
      printf("Error: %d contiguous bytes available\n", count);
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
   } else if (!strcmp(type, "B\n")) {
      bestFit(memory, pid, size);
   } else {
      printf("Error: Invalid argument\nOptions include:\n");
      printf("       F - first fit\n");
      printf("       W - worst fit\n");
      printf("       B - best fit\n");
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

void swap(int* a, int index1, int index2) {
   int temp = a[index2];
   a[index2] = a[index1];
   a[index1] = temp;
}

//TODO TODO TODO
// begins compaction process
// SUPER SUPER SLOW FOR LARGE NUMBERS ABOVE 100000
void compact(int* memory) {
   int i;
   int j;
   for(i = 0; i < MEM_SIZE; i++) {
      if(memory[i] == -1) {
         for(j = i; j < MEM_SIZE; j++) {
            if(memory[j] != -1) {
               swap(memory, i++, j);
            }
         }
      }
   }
}

// begins report process
void report(int* memory){
   int process = -1;
   int start = 0;
   int sFlag = 0;
   int i;
   printf("\n");
   //printIt(memory);
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
