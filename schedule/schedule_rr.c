#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "task.h"
#include "CPU.h"
#include "schedule_rr.h"

#define TIME_QUANTUM 10


// Lista de task
struct node *task_list = NULL;

// Função para adicionar uma task na fila
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->remaining_burst = burst;

    insert(&task_list, newTask);
}

// Função do escalonador Round Robin
void schedule() {
    while (task_list != NULL) {
        struct node *current = task_list;

        while (current != NULL) {
            Task *t = current->task;

           int exec_time;
            if (t->remaining_burst < TIME_QUANTUM) {
                 exec_time = t->remaining_burst;
            } else {
                 exec_time = TIME_QUANTUM;
                }               

            run(t, exec_time);
            t->remaining_burst -= exec_time;

            struct node *next = current->next;
            
            if (t->remaining_burst <= 0) {
                printf("✅ Task %s finalizada.\n", t->name);
                delete(&task_list, t);
            }

            current = next;
        }
    }
}
