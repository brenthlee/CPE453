/**
 * Various list operations
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "cpu.h"
#include "list.h"
#include "task.h"


// add a new task to the list of tasks
void insert(struct node **head, Task *newTask) {
    // add the new task to the list 
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    if (head == NULL) {
        //newNode->next = *head;
        newNode->next = NULL;
    } else {
        newNode->next = *head;
    }
    *head = newNode;
}

void addTail(struct node **head, Task *newTask) {
   ListNode *newNode = malloc(sizeof(ListNode));
   ListNode *cur = *head;
   newNode->task = newTask;
   newNode->next = NULL;
   if (cur == NULL) {
      *head = newNode;
   } else {
      while (cur->next != NULL) {
         cur = (cur->next != NULL) ? cur->next : cur;
      }
      cur->next = newNode;
   }
}

// delete the selected task from the list
void delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        *head = (*head)->next;
    }
    else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

// traverse the list
int traverse(struct node *head) {
    struct node *temp;
    temp = head;
    int count = 0;
    while (temp != NULL) {
        //printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
        //run(temp->task, temp->task->burst);
        temp = temp->next;
        count++;
    }
    return count;
}
