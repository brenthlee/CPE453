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

void initialize(int* arr, int size) {
   int i = 0;
   for (i; i < size; i++) {
      arr[i] = -1;
   }
}

void checkInput(char* input, int* arr) {
   //char copy[100];
   char* token;
   int i;
   int length = strlen(input);

   token = strtok(input, " ");
   while(token != NULL) {
      printf("%s\n", token);
      token = strtok(NULL, " ");
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
   max = atoi(argv[1]);
   int arr[max];
   initialize(arr, max);
   
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
         checkInput(input, arr);
      }
   }
}
