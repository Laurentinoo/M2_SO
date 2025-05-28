#ifndef SCHEDULE_RR_H
#define SCHEDULE_RR_H

// Adiciona uma task na lista
void add(char *name, int priority, int burst);

// Executa o escalonador Round Robin
void schedule();

#endif
