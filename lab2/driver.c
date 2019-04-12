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
        // add the task to the scheduler's list of tasks
        add(name,priority,burst,&cur);
        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    schedule(&cur);

    return 0;
}
