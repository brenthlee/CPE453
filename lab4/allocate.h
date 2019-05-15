#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARR_SIZE 1000
#define TRUE 1
#define FALSE 0

typedef struct process {
   int start;
   int end;
   char* name;
} process;

int MEM_SIZE = 0;
