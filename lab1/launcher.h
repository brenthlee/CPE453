#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "launcher.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include "cshell.h"

#define C 0
#define P 1
#define RD 0
#define WRT 1
#define MAX_ARGS 6

int execute(Stage stages[], int size, int ampFlag);
/*void runC1(Stage stage, int fd[]);*/
int loopMiddle(Stage stages[], int *wfd, int *rfd, int fd[], int size);
int launchMChild(Stage *stage, int wrt, int rd);
void runC1(int fd[], char *path, int flagVals[]);
int updateIO(Stage stage, int *wrt, int *rd);
int runFinal(Stage stage, int wfd, int rfd);
void closePipe(int fd);
void makePipe(int fd[]);
#endif
