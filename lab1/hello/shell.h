#ifndef SHELL_H
   #define SHELL_H
   
#define LEFTRDR 1
#define RIGHTRDR 2
#define PIPE 2

void parse(char* cmdLine, char* args[], char filename[], int* flag);
void stripEnt(char* comm);
void clear();
int updateIO(char* filename, int* wfd, int* rfd, int flag);

#endif
