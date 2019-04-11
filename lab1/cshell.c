#include "cshell.h"

int parseLine(Stage stages[], char *line, int *stageIndex, int* ampFlag)
{
   int commands = 0, error = 0;
   char *tok;

   tok = strtok(line, " ");
   while(tok != NULL && strcmp(tok, "\n"))
   {
      if (!strcmp(tok,"&")) {
         *ampFlag = 1;
      }
      tok = buildStage(tok, &(stages[*stageIndex]), &error, ampFlag);
      tok = strtok(NULL, " ");
      (*stageIndex)++;
      commands++;
      if(commands > MAXARGS)
      {
         return COMMAND2;
      }
      if(error != 0)
         return error;
   }
   return 0;
}

char *buildStage(char *tok, Stage *stage, int *error, int* ampFlag)
{
   int i = 0, dupCheck = 1;
   stage->argc = 0;
   while(tok != NULL)
   {
      if (!strcmp(tok,"&")) {
         *ampFlag = 1;
      }
      if(!strcmp(tok, "<") || !strcmp(tok, ">"))
      {
         tok = updateStage(tok, stage, error, ampFlag); /* assuming will return next */
         if(*error != 0)
         {
            return NULL;
         }
         dupCheck = 0; /* you just succeeded in reading an op w/o dupes */
      }

      if(tok == NULL || !strcmp(tok, "|"))
      {
         if(dupCheck == 1)
         {
            *error = SYNTAX;
            return NULL;
         }
         break;
      }

      if(strcmp(tok, "<") && strcmp(tok, ">"))
      {
         stage->args[i] = tok;
         stage->argc++;
         tok = strtok(NULL, " ");
      }
      if(stage->argc > 11)
      {
         *error = ARGS;
         return NULL;
      }
      i++;
      dupCheck = 0;
   }
   stage->args[stage->argc] = (char*)NULL; /* NULL terminate those arguments! */
   return tok;
}

char *updateStage(char *tok, Stage *stage, int *error, int* ampFlag)
{
   char *fileStr = strtok(NULL, " ");
      if (!strcmp(tok,"&")) {
         *ampFlag = 1;
      }
   if(!strcmp(fileStr, "|") || !strcmp(fileStr, "<") || !strcmp(fileStr, ">"))
   {
      /* handles double ops after a redirect */
      *error = SYNTAX;
      return NULL;
   }
   else if(!strcmp(tok, ">"))
   {
      stage->output = fileStr;
      stage->hasFile = 1;
   }
   else if(!strcmp(tok, "<"))
   {
      stage->input = fileStr;
      stage->hasFile = 1;
   }
   fileStr = strtok(NULL, " ");
   return fileStr;
}
