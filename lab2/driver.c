/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "task.h"
#include "list.h"
#include "schedulers.h"

#define SIZE    100

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    int tid = 0; //EXTRA CREDIT

    if (!argv[1]) {
        fprintf(stderr, "No file given\n");
        exit(EXIT_FAILURE);
    }
    in = fopen(argv[1],"r");
    ListNode* cur = NULL;
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));
        add(name,priority,burst,&cur, tid);
        __sync_fetch_and_add(&tid, 1); //EXTRA CREDIT
    }
    fclose(in);

    // invoke the scheduler
    schedule(cur);
    freeNodes(&cur);
    return 0;
}
