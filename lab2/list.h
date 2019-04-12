/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

typedef struct node {
    Task *task;
    struct node *next;
} ListNode;

// insert and delete operations.
void insert(struct node **head, Task *task);
void addTail(struct node **head, Task *newTask);
void delete(struct node **head, Task *task);
void traverse(struct node *head);
