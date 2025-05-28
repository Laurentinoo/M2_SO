//EDF Backup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "list.h"
#include "task.h"
#include "CPU.h"
#include "schedule_edf.h"

#define TIME_QUANTUM 10

// Lista de tarefas
struct node *task_list = NULL;

// Tempo global
int clock_time = 0;

// Tick gerado pela thread timer
volatile int tick = 0;

// Função da thread que simula o timer de hardware
void *timer_thread(void *arg) {
    int interval = *((int *)arg);

    while (1) {
        sleep(interval); // Simula um tick 
        tick = 1;
    }
    return NULL;
}

// Função para adicionar uma task (com deadline)
void add_edf(char *name, int priority, int burst, int deadline) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->remaining_burst = burst;
    newTask->deadline = deadline;
    newTask->start_time = clock_time; // Marca quando foi criada-------------------------

    insert(&task_list, newTask);
}

// Função auxiliar para buscar a task com menor deadline
Task *get_deadline() {
    if (task_list != NULL){

    struct node *temp = task_list;
    Task *deadline = temp->task;

    while (temp != NULL) {
        if (temp->task->deadline < deadline->deadline) {
            deadline = temp->task;
        }
        temp = temp->next;
    }

        return deadline;
    }
    return 0;
}

// Função principal do escalonador EDF
void schedule() {
    pthread_t timer;
    int interval = 1; // 1 segundo 

    pthread_create(&timer, NULL, timer_thread, &interval);

    while (task_list != NULL) {
        if (tick) {
            int exec_time;
            clock_time+=exec_time;
            tick = 0;

            printf("===== Clock Time: %d =====\n", clock_time);

            // Verificar deadlines
            struct node *temp = task_list;
            while (temp != NULL) {
                if (clock_time > temp->task->deadline) {
                    printf("❌  Task [%s] perdeu o deadline! (Deadline: %d)\n", 
                           temp->task->name, temp->task->deadline);
                }
                temp = temp->next;
            }

            // Seleciona a task com menor deadline
            Task *t = get_deadline();
       
                if (t->remaining_burst < TIME_QUANTUM) {
                    exec_time = t->remaining_burst;
                 } else {
                     exec_time = TIME_QUANTUM;
                    }

            run_edf(t, exec_time);
            t->remaining_burst -= exec_time;

            if (t->remaining_burst <= 0) {
                printf("✅ Task [%s] finalizada.\n", t->name);
                delete(&task_list, t);
            }
        }
    }

    // Encerra o timer
    pthread_cancel(timer);
}
