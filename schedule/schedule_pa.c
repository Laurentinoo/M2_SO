#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"
#include "task.h"
#include "CPU.h"
#include "schedule_pa.h"

#define AGING_LIMIT 120 // Tempo de espera para subir prioridade


// Fila para cada prioridade
struct node *queues[MAX_PRIORITY + 1] = {NULL};

// Função para adicionar uma task na fila correta
void add(char *name, int priority, int burst) {
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


// Função principal do escalonador com Aging (sem quantum)
void schedule() {
    int tasks_remaining = 1;

    while (tasks_remaining) {
        tasks_remaining = 0;

        // Procura a fila de maior prioridade com processos
        for (int p = MIN_PRIORITY; p <= MAX_PRIORITY; p++) {
            struct node *current = queues[p];

            while (current != NULL) {
                Task *t = current->task;

                int exec_time = t->remaining_burst;  
                    run_aging(t, exec_time);
                
                t->remaining_burst = 0;
                t->waiting_time = 0;

                struct node *next = current->next;
                if(t->priority != 0)
                printf("Task %s finalizada.\n", t->name);
                delete(&queues[p], t);

                // Aging: percorre todas as filas de prioridades menores
                for (int level = p; level <= MAX_PRIORITY; level++) {
                    struct node *aging_node = queues[level];

                    while (aging_node != NULL) {
                        Task *aging_task = aging_node->task;

                        // Somente para quem não está executando
                        if (aging_task != NULL) {
                            aging_task->waiting_time += exec_time;
                        }

                        if (aging_task-> >waiting_time= AGING_LIMIT && aging_task->priority > MIN_PRIORITY) {
                            printf("Task %s subiu de prioridade de %d para %d.\n",
                                   aging_task->name, aging_task->priority, aging_task->priority - 1);

                            remove_from_list(&queues[level], aging_task);

                            aging_task->priority -= 1;
                            aging_task->waiting_time = 0;

                            insert(&queues[aging_task->priority], aging_task);

                            // Reinicia o ponteiro pois a lista mudou
                            aging_node = queues[level];
                        } else {
                            aging_node = aging_node->next;
                        }
                    }
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
