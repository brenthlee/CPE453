/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "shell.h"

#define MAX_LINE		80 /* 80 chars per line, per command */
int main(void) {
    char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;
    int flag = -1;   // indicates LEFTRDR, RIGHTRDR, PIPE 
    int error = -1;
    int wfd = STDOUT_FILENO, rfd = STDIN_FILENO;
    char filename[20];
    char cmdLine[MAX_LINE];
    pid_t pid;

   	
    while (should_run){
        memset(args,0,MAX_LINE);
        flag = -1;
        printf("osh> ");
        fflush(stdout);
        if (fgets(cmdLine, MAX_LINE+2, stdin) == NULL) {
            should_run = 0;
        } else if (!(strcmp(cmdLine, "exit\n"))) {
            should_run = 0;
        } else if (strlen(cmdLine) > MAX_LINE) {
            clear();
            fprintf(stderr, "shell: Command line too long\n");
        }
        else {
            parse(cmdLine, args, filename, &flag);
            printf("%d\n", flag);
            if((pid = fork()) < 0) {
                perror(NULL);
                exit(1);
            } else if (pid == 0) {
                printf("args: %s\n", args[0]);
                // if there is a redirect, change file descriptors
                if (flag != -1) {
                    filename[strlen(filename)-1] = 0;
                    error = updateIO(filename, &wfd, &rfd, flag);
                }
                if(dup2(wfd, STDOUT_FILENO) < 0)
                    fprintf(stderr, "dup2 failed on line %d\n", __LINE__);
                if(dup2(rfd, STDIN_FILENO) < 0)
                    fprintf(stderr, "dup2 failed on line %d\n", __LINE__);
                if (execvp(args[0],args) < 0) {
                    printf("failure\n");
                }
            } else {
                waitpid(pid, NULL, 0);
            }
        	/**
         	 * After reading user input, the steps are:
         	 * (1) fork a child process
         	 * (2) the child process will invoke execvp()
         	 * (3) if command includes &, parent and child will run concurrently
         	 */
    	}
    }
    return 0;
}

void clear() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void parse(char* cmdLine, char* args[], char filename[], int* flag) {
    int i = 0, j = 0;
    int ctr = 0;
    char* cur;
    //int flag = -1;
    cur = strtok(cmdLine, " ");
    while (cur != NULL) {
        if (cur[0] == '<') {
            //args[ctr++] = "LEFT";
            cur = strtok(NULL," ");
            if (cur != NULL) {
                strncpy(filename, cur, strlen(cur));
            } else {
                fprintf(stderr, "no file after left\n");
            }
            *flag = LEFTRDR;
            break;
        } else if (cur[0] == '>') {
            //args[ctr++] = "RIGHT";
            cur = strtok(NULL," ");
            if (cur != NULL) {
                strncpy(filename, cur, strlen(cur));
            } else {
                fprintf(stderr, "no file after right\n");
            }
            *flag = RIGHTRDR;
            break;
        } else if (cur != NULL) {
            args[ctr++] = cur;
        }
        cur = strtok(NULL, " ");
    }
    stripEnt(args[ctr-1]);
    for(i=0; i < ctr; i++) {
        printf("%s\n", args[i]);
    }
}

void stripEnt(char *comm) {
    if(comm[strlen(comm)-1] == '\n') {
        comm[strlen(comm)-1] = 0;
    }
}

int updateIO(char* filename, int* wfd, int* rfd, int flag) {
    if(flag == RIGHTRDR) {
        *wfd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if(*wfd < 0) {
            exit(1);
        }
    }
    if(flag == RIGHTRDR) {
        *rfd = open(filename, O_RDONLY);
        if(*rfd < 0) {
            exit(1);
        }
    }
    return 0;
}
