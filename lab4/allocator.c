#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct Process {
   int start;
   int end;
   int isHole;
   char* name;
   struct Process *next;
} Process;

void worstFit(Process** head, char* process, int pSize) {
   Process* cur = *head;
   Process* prev = NULL;
   int space = 0;
   int i = 0;
   int mIndex;
   while (cur != NULL) {
      if (cur->isHole == 1 && ((cur->end - cur->start + 1) > space)) {
         space = cur->end - cur->start + 1;
         mIndex = i;
      }
      i++;
      prev = cur;
      cur = cur->next;
   }
   if (space >= pSize) {
      cur = *head;
      for (i = 0; i < mIndex; i++) {
         prev = cur;
         cur = cur->next;
      }
      
      Process* newProcess = (Process*)malloc(sizeof(Process));
      newProcess->name = (char*)malloc(sizeof(char)*20);
      strcpy(newProcess->name, process);
      newProcess->start = cur->start;
      newProcess->end = newProcess->start + pSize - 1;
      newProcess->isHole = 0;
      if (cur->start == 0) {
         if (cur->next != NULL) {
            cur->start = newProcess->end + 1;
            newProcess->next = cur;
            (*head) = newProcess;
            if (cur->start > cur->end) {
               newProcess->next = cur->next;
               free(cur);
            }
         } else {
            cur->start = newProcess->end + 1;
            newProcess->next = cur;
            (*head) = newProcess;
            if (cur->start > cur->end) {
               newProcess->next = NULL;
               free(cur);
            }
         }
      } else {
         if (cur->next != NULL) {
            cur->start = newProcess->end + 1;
            prev->next = newProcess;
            newProcess->next = cur;
            if (cur->start > cur->end) {
               newProcess->next = cur->next;
               free(cur);
            }
         } else {
            cur->start = newProcess->end + 1;
            prev->next = newProcess;
            newProcess->next = cur;
            if (cur->start > cur->end) {
               newProcess->next = NULL;
               free(cur);
            }
         }
      }
   } else {
      printf("Error: %d contiguous bytes available\n", space);
      printf("       %d bytes requested\n", pSize);
   }
}

void bestFit(Process** head, char* process, int pSize) {
   Process* cur = *head;
   Process* prev = NULL;
   int space = -1;
   int mIndex = -1;
   int maxSpace = 0;
   int i;
   int tmpSize = -1;
   while (cur != NULL) {
      if (cur->isHole) {
         tmpSize = cur->end - cur->start + 1;
         if (tmpSize > maxSpace) {
            maxSpace = tmpSize;
         }
         if (space < 0 && tmpSize >= pSize) {
            space = tmpSize;
            mIndex = i;
         } else if (tmpSize < space && tmpSize >= pSize) {
            space = tmpSize;
            mIndex = i;
         }
      }
      i++;
      cur = cur->next;
   }
   if (mIndex == -1) {
      printf("Error: %d contiguous bytes available\n", maxSpace);
      printf("       %d bytes requested\n", pSize);
   } else {
      cur = *head;
      for (i = 0; i < mIndex; i++) {
         prev = cur;
         cur = cur->next;
      }
      Process* newProcess = (Process*)malloc(sizeof(Process));
      newProcess->name = (char*)malloc(sizeof(char)*20);
      strcpy(newProcess->name, process);
      newProcess->start = cur->start;
      newProcess->end = newProcess->start + pSize - 1;
      newProcess->isHole = 0;
      newProcess->next = cur;
      if (cur->start == 0) {
         if (cur->next != NULL) {
            cur->start = newProcess->end + 1;
            (*head) = newProcess;
            if (cur->start > cur->end) {
               newProcess->next = cur->next;
               free(cur);
            }
         } else {
            cur->start = newProcess->end + 1;
            (*head) = newProcess;
            if (cur->start > cur->end) {
               newProcess->next = NULL;
               free(cur);
            }
         }
      } else {
         if (cur->next != NULL) {
            cur->start = newProcess->end + 1;
            prev->next = newProcess;
            if (cur->start > cur->end) {
               newProcess->next = cur->next;
               free(cur);
            }
         } else {
            cur->start = newProcess->end + 1;
            prev->next = newProcess;
            if (cur->start > cur->end) {
               newProcess->next = NULL;
               free(cur);
            }
         }
      }
   }
}

void release(Process** head, char* process) {
   Process* cur = *head;
   Process* prev = NULL;
   int flag = 0;
   while (cur != NULL) {
      if (!strcmp(cur->name, process)) {
         flag = 1;
         cur->name = "Unused";
         cur->isHole = 1;
         if (cur->next != NULL && cur->next->isHole) {
            cur->next->start = cur->start;
            if (prev == NULL) {
               (*head) = cur->next;
            } else {
               prev->next = cur->next;
            }
            if (!(prev != NULL && prev->isHole)) {
               free(cur);
            } else {
               cur = cur->next;
            }
         }
         if (prev != NULL && prev->isHole) {
            prev->end = cur->end;
            if (cur->next == NULL) {
               prev->next = NULL;
            } else {
               prev->next = cur->next;
            }
            free(cur);
         }
      }
      prev = cur;
      cur = cur->next;
   }
   if (!flag) {
      printf("Error: %s has not been allocated\n", process);
   }
}

void compact(Process** head) {
   int count = 0;
   Process* cur = *head;
   Process* prev = NULL;
   while (cur != NULL) {
      if (cur->isHole) {
         if (prev == NULL) {
            if (cur->next == NULL) {
            } else {
               count = count + (cur->end - cur->start + 1);
               cur->next->start = cur->next->start - count;
               cur->next->end = cur->next->end - count;
               (*head) = cur->next;
            }
         } else {
            if (cur->next == NULL) {
            } else {
               count = count + (cur->end - cur->start + 1);
               cur->next->start = cur->next->start - count;
               cur->next->end = cur->next->end - count;
               prev->next = cur->next;
            }
         }
      } else {
         if (prev != NULL && (!(prev->isHole))) {
            cur->start = cur->start - count;
            cur->end = cur->end - count;
         }
      }
      prev = cur;
      cur = cur->next;
   }
   if (prev != NULL) {
      if (prev->isHole) {
         prev->start = prev->start - count;
      } else {
         Process* newProcess = (Process*)malloc(sizeof(Process));
         newProcess->name = (char*)malloc(sizeof(char)*20);
         newProcess->name = "Unused";
         newProcess->start = prev->end+1;
         newProcess->end = newProcess->start+count-1;
         newProcess->isHole = 1;
         newProcess->next = NULL;
         prev->next = newProcess;
      }
   }
}

void report(Process* head) {
   Process* cur = head;
   while (cur != NULL) {
      if (!strcmp("Unused", cur->name)) {
         printf("Addresses [%d:%d] %s\n",cur->start,cur->end,cur->name);
      } else {
         printf("Addresses [%d:%d] Process %s\n",cur->start,cur->end,cur->name);
      }
      cur = cur->next;
   }
}

void firstFit(Process** head, char* process, int pSize) {
   Process* cur = *head;
   Process* prev = NULL;
   int space = 0;
   int flag = 0;
   while (cur != NULL) {
      if (cur->isHole == 1) {
         space = cur->end - cur->start + 1;
         if (space >= pSize) {
            flag = 1;
            break;
         }
      }
      prev = cur;
      cur = cur->next;
   }
   if (flag) {
      Process* newProcess = (Process*)malloc(sizeof(Process));
      newProcess->name = (char*)malloc(sizeof(char)*20);
      strcpy(newProcess->name, process);
      newProcess->start = cur->start;
      newProcess->end = newProcess->start + pSize - 1;
      newProcess->isHole = 0;
      newProcess->next = NULL;
      if (cur->start == 0) {
         cur->start = newProcess->end+1;
         newProcess->next = (*head);
         (*head) = newProcess;
         if (cur->start > cur->end) {
            newProcess->next = NULL;
            free(cur);
         }
      } else {
         cur->start = newProcess->end+1;
         newProcess->next = cur;
         prev->next = newProcess;
         if (cur->start > cur->end) {
            if (cur->next != NULL) {
               newProcess->next = cur->next;
            } else {
               newProcess->next = NULL;
            }
            free(cur);
         }
      }
   } else {
      printf("Error: %d contiguous bytes available\n", space);
      printf("       %d bytes requested\n", pSize);
   }
}

void request(Process** head, char* process, int pSize, char* type) {
   int flag = 0;
   Process* cur = *head;
   while (cur != NULL) {
      if (!strcmp(process, cur->name)) {
         printf("Error: Process already allocated\n");
         flag = 1;
         break;
      }
      cur = cur->next;
   }
   if (!flag) {
      if (!strcmp(type, "F\n")) {
         firstFit(head, process, pSize);
      } else if (!strcmp(type, "W\n")) {
         worstFit(head, process, pSize);
      } else if (!strcmp(type, "B\n")) {
         bestFit(head, process, pSize);
      } else {
         printf("Error: Invalid arument\nOptions include:\n");
         printf("       F - first fit\n");
         printf("       W - worst fit\n");
         printf("       B - best fit\n");
      }
   }
}

void checkInput(char* input, Process** head) {
   char* token;
   char* process;
   char* size;
   char* type;
   token = strtok(input, " ");
   if (!strcmp("RQ", token)) {
      process = strtok(NULL, " ");
      size = strtok(NULL, " ");
      type = strtok(NULL, " ");
      request(head, process, atoi(size), type);
   } else if (!strcmp("RL", token)) {
      process = strtok(NULL, " ");
      process[strlen(process)-1] = '\0';
      release(head, process);
   } else if (!strcmp("C\n", token)) {
      compact(head);
   } else if (!strcmp("STAT\n", token)) {
      report(*head);
   } else if (!strcmp("X\n", token)) {
      exit(1);
   } else {
      printf("Invalid option. Options include:\n");
      printf("\trequest: RQ P# #bytes Strategy(F,B,W)\n");
      printf("\trelease: RL P#\n");
      printf("\tcompact: C\n");
      printf("\tsummary: STAT\n");
      printf("\t   quit: X\n\n");
   }
}

int main(int argc, char *argv[]) {
   char input[100];
   if (argc < 2) {
      printf("Error: Too few arguments\nUsage: ./allocator <mem size>\n");
      exit(1);
   } else if (argc > 2) {
      printf("Error: Too many arguments\nUsage: ./allocator <mem size>\n");
      exit(1);
   }
   Process* head = (Process*)malloc(sizeof(Process));
   head->start = 0;
   head->end = atoi(argv[1])-1;
   head->isHole = 1;
   head->name = (char*)malloc(sizeof(char)*20);
   head->name = "Unused";
   head->next = NULL;
   while (1) {
      printf("allocator> ");
      if (!fgets(input, 100, stdin)) {
         if (feof(stdin)) {
            exit(1);
         }
      } else {
         checkInput(input, &head);
      }
   }
}
