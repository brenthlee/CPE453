#include "cshell.h"

int checkPipes(char *str, int len)
{
   int err = 0;
   if(str[0] == '|' || str[len - 2] == '|')
   {
      printError(-1, NULL);
      err = -1;
   }
   else if(str[len-2] == '<' || str[len - 2] == '>')
   {
      printError(-2, NULL);
      err = -1;
   }
   return err;
}

int checkValid(char *old, char *tok)
{
   int err = 0;
   if(isOp(old) && isOp(tok))
   {
      printError(-2, NULL);
      err = -1;
   }
   return err;
}

int isOp(char *tok)
{
   if(!strcmp(tok, "|") || !strcmp(tok, "<") || !strcmp(tok, ">"))
   {
      return 1;
   }
   return 0;
}

void printError(int code, char *comm)
{
   fprintf(stderr, "cshell: ");
   if(code == PIPE)
   {
      fprintf(stderr, "Invalid pipe\n");
   }
   else if(code == SYNTAX)
   {
      fprintf(stderr, "Syntax error\n");
   }
   else if(code == COMMAND2)
   {
      fprintf(stderr, "Too many commands\n");
   }
   else if(code == COMMAND)
   {
      stripEnt(comm);
      fprintf(stderr, "%s: Command not found\n", comm);
   }
   else if(code == FILES)
   {
      fprintf(stderr, "Unable to open file for input\n");
   }
   else if(code == ARGS)
   {
      stripEnt(comm);
      fprintf(stderr, "%s: Too many arguments\n", comm);
   }
}

void stripEnt(char *comm)
{
   if(comm[strlen(comm)-1] == '\n')
   {
      comm[strlen(comm)-1] = 0;
   }
}
