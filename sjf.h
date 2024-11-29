#ifndef SJF_H
#define SJF_H

#include "list.h"

// Resultado de SJF
typedef struct {
    float avg_waiting_time;
    float avg_turnaround_time;
} SJFResult;

// Prototipos
void sortListByBurst(List *list); // Ordena la lista por burst
SJFResult calculateSJF(List *list);
void printSJFDetails(List *list);
void printSJFGantt(List *list);
#endif // SJF_H
