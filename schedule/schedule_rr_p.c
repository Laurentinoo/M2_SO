#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "task.h"
#include "CPU.h"
#include "schedule_rr_p.h"

#define TIME_QUANTUM 10


// Uma lista para cada prioridade
struct node *queues[MAX_PRIORITY + 1];  // Índice de 1 a 10

// Adiciona uma task na fila correspondente à sua prioridade
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->remaining_burst = burst;

    insert(&queues[priority], newTask);
}

// Função principal do escalonador RR_P
void schedule() {
    int tasks_remaining = 1;

    while (tasks_remaining) {
        tasks_remaining = 0;

        for (int p = MIN_PRIORITY; p <= MAX_PRIORITY; p++) {
            struct node *current = queues[p];

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
                    delete(&queues[p], t);
                }

                current = next;
            }

            if (queues[p] != NULL) {
                tasks_remaining = 1;
                break;
            }
        }
    }
}
