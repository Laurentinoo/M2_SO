/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "task.h"

// run this task for the specified time slice
void run(Task *task, int slice) {
    printf("Running task = [%s] [Priority: %d] [Burst: %d] [Remaining: %d] for %d units.\n",
           task->name, task->priority, task->burst, task->remaining_burst, slice);
}

void run_edf(Task *task, int slice) {
    printf("Running task = [%s] [Priority: %d] [Burst: %d] [Remaining: %d] [Deadline: %d] for %d units.\n",
           task->name, task->priority, task->burst, task->remaining_burst, task->deadline, slice);
}

void run_aging(Task *task, int slice) {
    printf("Running task = [%s] [Priority: %d] [Burst: %d] [Remaining: %d] [Waiting: %d] for %d units.\n",
           task->name, task->priority, task->burst, task->remaining_burst, task->waiting_time, slice);
}



