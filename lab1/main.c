#include "cshell.h"
#include "launcher.h"

int main(int argc, char *argv[])
{
   char    line[84];
   int     status = -1, stageIndex = 0;
   char    history[84];
   Stage   array[20];
   memset(array, 0, sizeof(Stage)*20);
   setbuf(stdout, NULL);
   int count = 0;
   int ampFlag = 0;
   while(status)
   {
      printf("osh> ");
      if(!fgets(line, 84, stdin))
      {
         if(feof(stdin))
         {
            printf("exit\n");
            break;
         }
      } else {
         if (!strcmp(line, "!!\n")) {
            if (count == 0) {
               fprintf(stderr, "No recent commands\n");
            } else {
               strncpy(line, history, sizeof(history));
            }
         }
         if ((!strcmp(line, "!!\n") && count != 0) || (strcmp(line,"!!\n"))) {
            if(strlen(line) > MAXLINE) {
               strncpy(history, line, sizeof(line));
               count++;
               fprintf(stderr, "Command line too long\n");
               clear();
            }
            else if(!strcmp(line, "exit\n"))
            {
               exit(0);
            }
            else if(!checkPipes(line, strlen(line)))
            {
               count++;
               strncpy(history, line, sizeof(line));
               parseNexec(array, line, &stageIndex, ampFlag);
            } else {
               fprintf(stderr, "there is a syntax error\n");
            }
         }
      }
   }
   return 0;
}

void parseNexec(Stage stages[], char *line, int *stageIndex, int ampFlag)
{
   int error;
   stripEnt(line); /* strip off \n */
   error = parseLine(stages, line, stageIndex, &ampFlag);
   if(error != 0)
   {
      printError(error, stages[*stageIndex-1].args[0]);
   }
   else
   {
      execute(stages, *stageIndex, ampFlag);
   }
   *stageIndex = 0;
   memset(stages, 0, sizeof(Stage)*20);
}

void clear() {
   int c;
   while ((c = getchar()) != EOF && (c!='\n'));
}
