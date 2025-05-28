#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"
#include "task.h"
#include "CPU.h"
#include "schedule_pa.h"

#define TIME_QUANTUM 10
#define AGING_LIMIT 100 // Tempo de espera para subir prioridade

// Fila para cada prioridade
struct node *queues[MAX_PRIORITY + 1] = {NULL};

// Adiciona uma tarefa na fila correspondente
void add_aging(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->remaining_burst = burst;
    newTask->start_time = 0;
    newTask->deadline = 0;
    newTask->waiting_time = 0;

    insert(&queues[priority], newTask);
}


// Função principal do escalonador com Aging
void schedule_aging() {
    int tasks_remaining = 1;

    while (tasks_remaining) {
        tasks_remaining = 0;

        for (int p = MIN_PRIORITY; p <= MAX_PRIORITY; p++) {
            struct node *current = queues[p];

            // Executa tarefas da prioridade atual
            while (current != NULL) {
                Task *t = current->task;

                int exec_time = (t->remaining_burst < TIME_QUANTUM) ? t->remaining_burst : TIME_QUANTUM;

                run(t, exec_time);

                t->remaining_burst -= exec_time;
                t->waiting_time = 0; // Reset após executar

                struct node *next = current->next;

                if (t->remaining_burst <= 0) {
                    printf("Task %s finalizada.\n", t->name);
                    delete(&queues[p], t);
                }

                current = next;
            }

            // Aging para tarefas de prioridades menores
            for (int level = p + 1; level <= MAX_PRIORITY; level++) {
                struct node *aging_node = queues[level];

                while (aging_node != NULL) {
                    if (aging_node->task == NULL) {
                        aging_node = aging_node->next;
                        continue;
                    }

                    Task *aging_task = aging_node->task;

                    aging_task->waiting_time += TIME_QUANTUM;

                    if (aging_task->waiting_time >= AGING_LIMIT) {
                        printf("Task %s subiu de prioridade de %d para %d por aging.\n",
                               aging_task->name, aging_task->priority, aging_task->priority - 1);

                        remove_from_list(&queues[level], aging_task);

                        aging_task->priority -= 1;
                        if (aging_task->priority < MIN_PRIORITY) {
                            aging_task->priority = MIN_PRIORITY;
                        }
                        aging_task->waiting_time = 0;

                        insert(&queues[aging_task->priority], aging_task);

                        // Reinicia o ponteiro da lista porque ela foi alterada
                        aging_node = queues[level];
                    } else {
                        aging_node = aging_node->next;
                    }
                }
            }

            if (queues[p] != NULL) {
                tasks_remaining = 1;
                break;
            }
        }
    }
}