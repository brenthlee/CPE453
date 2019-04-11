#include "launcher.h"
#include "cshell.h"
#include <sys/types.h>
#define RD 0
#define WRT 1

int execute(Stage stages[], int size, int ampFlag)
{
   int fd[2], rfd, wfd, error = 0, status = -1;
   /* printf("stages 1: %s\n", stages[1].args[0]);*/

   rfd = STDIN_FILENO;
   if(size > 1)
   {
      /* loop through middle children, IF any */
      if((error = loopMiddle(stages, &wfd, &rfd, fd, size) != 0))
      {
         return error;
      }
      /*fprintf(stderr, "closing %d after loop\n", wfd);*/
      closePipe(wfd);
   }

   wfd = STDOUT_FILENO;
   runFinal(stages[size-1], wfd, rfd);

   if(rfd != 0)
      closePipe(rfd);

   while(size > 0)
   {
      /*printf("waiting\n");*/
     if (ampFlag == 0) {
         wait(&status);
     } 
     --(size);
   }
   return 0;
}

int loopMiddle(Stage stages[], int *wfd, int *rfd, int fd[], int size)
{
   pid_t pid;
   int i;
   for(i = 0; i < size - 1; i++)
   {
      /*printf(" IN LOOP: %s\n", (stages[i]).args[0]); */
      if(i != 0)
      {
         closePipe(*wfd);
      }
      makePipe(fd);
      *wfd = fd[1];

      if((pid = fork()) < 0)
      {
         perror(NULL);
         exit(1);
      }
      else if(pid == 0)
      {
         /*fprintf(stderr, "closing %d on i of %d in child\n", fd[RD], i);*/
         closePipe(fd[RD]); /* close the next child's read end */
         launchMChild(&(stages[i]), *wfd, *rfd);
         printError(COMMAND, (stages[i]).args[0]);
         exit(EXIT_FAILURE);
      }
      else
      {
         if(i != 0)
         {
            /*fprintf(stderr, "closing %d on i of %d in parent\n", *rfd, i);*/
            closePipe(*rfd);
         }
         *rfd = fd[RD];
      }
   }
   return 0;
}

int launchMChild(Stage *stage, int wrt, int rd)
{
   int error;
   if(stage->hasFile)
   {
      error = updateIO(*stage, &wrt, &rd);
      if(error)
      {
         printError(FILES, NULL);
         exit(1);
      }
   }
   /*fprintf(stderr, "duping %d on line %d\n", wrt, __LINE__);*/
   if(dup2(wrt, STDOUT_FILENO) < 0)
      fprintf(stderr, "dup2 failed on line %d\n", __LINE__);
   /*fprintf(stderr, "duping %d on line %d\n",rd,  __LINE__);*/
   if(dup2(rd, STDIN_FILENO) < 0)
      fprintf(stderr, "dup2 failed on line %d\n", __LINE__);
   execvp(stage->args[0], stage->args);
   return COMMAND;
}

int updateIO(Stage stage, int *wrt, int *rd)
{
   /*fprintf(stderr, "output: %s input: %s\n", stage.output, stage.input);*/
   if(stage.output != 0)
   {
      *wrt = open(stage.output, O_CREAT | O_WRONLY | O_TRUNC, 0666);
      if(*wrt < 0)
      {
         /*fprintf(stderr, "open wrt failed\n");*/
         return FILES;
      }
   }
   if(stage.input != 0)
   {
      *rd = open(stage.input, O_RDONLY);
      if(*rd < 0)
      {
         /*fprintf(stderr, "open rd failed\n");*/
         return FILES;
      }
   }
   /*fprintf(stderr, "wrt: %d rd: %d\n", *wrt, *rd);*/
   return 0;
}

void closePipe(int fd)
{
   if(close(fd) == -1)
   {
      perror(NULL);
      exit(1);
   }
}

void makePipe(int fd[])
{
   if(pipe(fd) == -1)
   {
      exit(EXIT_FAILURE);
   }
}

int runFinal(Stage stage, int wfd, int rfd)
{
   pid_t pid;
   int error;

   /*fprintf(stderr, "final writing: %d reading: %d\n", wfd, rfd);*/
   if((pid = fork()) < 0)
   {
      perror(NULL);
      exit(1);
   }
   else if(pid == 0)
   {
      if(stage.hasFile)
      {
         error = updateIO(stage, &wfd, &rfd);
         if(error)
         {
            printError(FILES, NULL);
            exit(1);
         }
         /*fprintf(stderr, "wfd: %d rfd: %d on line %d", wfd, rfd, __LINE__);*/
      }

      /*fprintf(stderr, "duping %d on line %d\n", wfd, __LINE__);*/
      if(dup2(wfd, STDOUT_FILENO) < 0)
         fprintf(stderr, "dup2 failed on line %d\n", __LINE__);

      /*fprintf(stderr, "duping %d on line %d\n", rfd, __LINE__);*/
      if(dup2(rfd, STDIN_FILENO) < 0)
         fprintf(stderr, "dup2 failed on line %d\n", __LINE__);

      execvp(stage.args[0], stage.args);
      printError(COMMAND, stage.args[0]);
      exit(EXIT_FAILURE);
   }
   return 0;
}
