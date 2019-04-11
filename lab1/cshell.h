#ifndef CSHELL_H
#define CSHELL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#define MAXLINE 80
#define MAXARGS 40
#define MAXPIPES 20

#define PIPE -1
#define SYNTAX -2
#define COMMAND -3
#define COMMAND2 -4
#define FILES -5
#define ARGS -6
#define EXEC -7

typedef struct
{
   size_t argc;
   char *args[10];
   char *input;
   char *output;
   int hasFile;
} Stage;

int parseLine(Stage args[], char *line, int *stageIndex, int* ampFlag);
char *get(Stage stages[], int index, char *type, char *arg0, int *status);
char *buildStage(char *tok, Stage *stage, int *status, int *ampFlag);
char *updateStage(char *tok, Stage *stage, int *status, int *ampFlag);
void parseNexec(Stage stages[], char *line, int *stageIndex, int ampFlag);

int checkPipes(char *str, int len);
int checkValid(char *old, char *tok);
int isOp(char *tok);
void printError(int code, char *comm);
void stripEnt(char *comm);
void clear();

#endif
