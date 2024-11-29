#ifndef RR_H
#define RR_H

#include "list.h"

// Resultado de Round Robin
typedef struct {
    float avg_waiting_time;
    float avg_turnaround_time;
} RRResult;

// Prototipos
RRResult roundRobin(List *list, int quantum);
void printRoundRobinDetails(List *list, int quantum);

#endif // RR_H
