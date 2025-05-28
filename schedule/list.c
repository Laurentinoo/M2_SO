/**
 * Various list operations
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"


// add a new task to the list of tasks
void insert(struct node **head, Task *newTask) {
    // add the new task to the list 
    struct node *newNode = malloc(sizeof(struct node));

    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
}

void delete(struct node **head, Task *task) {
    struct node *temp = *head;
    struct node *prev = NULL;

    while (temp != NULL) {
        if (task == temp->task) { 
            if (prev == NULL) {
                *head = temp->next;
            } else {
                prev->next = temp->next;
            }

            free(temp->task->name);
            free(temp->task);
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}



// traverse the list
void traverse(struct node *head) {
    struct node *temp;
    temp = head;

    while (temp != NULL) {
        printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
}


void remove_from_list(struct node **head, Task *task) {
    struct node *temp = *head;
    struct node *prev = NULL;

    while (temp != NULL) {
        if (strcmp(task->name, temp->task->name) == 0) {
            if (prev == NULL) {
                *head = temp->next;
            } else {
                prev->next = temp->next;
            }

            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}
