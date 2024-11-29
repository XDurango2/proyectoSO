// fcfs.c
#include "fcfs.h"
#include <stdio.h>

void initializeFCFS(List *list) {
    initializeList(list);
}

FCFSResult calculateFCFS(List *list) {
    if (isListEmpty(list)) {
        printf("No hay procesos en la lista.\n");
        return (FCFSResult){0, 0};
    }

    int N = list->size;
    int at[N], bt[N], wt[N], tat[N], ct[N];  // Agregado ct[] para completion time
    Node *current = list->head;

    // Obtener burst times
    for (int i = 0; i < N; i++) {
        bt[i] = current->burst;
        at[i] = 0;  // Asumiendo que todos los procesos llegan al tiempo 0
        current = current->next;
    }

    // Calcular completion time y waiting time
    ct[0] = bt[0];  // Primer proceso
    wt[0] = 0;      // Primer proceso no espera
    
    for (int i = 1; i < N; i++) {
        ct[i] = ct[i-1] + bt[i];    // Tiempo de finalización
        wt[i] = ct[i-1] - at[i];    // Tiempo de espera
    }

    // Calcular turnaround time
    for (int i = 0; i < N; i++) {
        tat[i] = ct[i] - at[i];  // TAT = Completion Time - Arrival Time
    }

    // Calcular promedios
    float sum_wt = 0, sum_tat = 0;
    for (int i = 0; i < N; i++) {
        sum_wt += wt[i];
        sum_tat += tat[i];
    }

    return (FCFSResult){
        .avg_wt = sum_wt / N,
        .avg_tat = sum_tat / N
    };
}

void printFCFSGantt(List *list) {
    if (isListEmpty(list)) {
        printf("No hay procesos para mostrar en el diagrama de Gantt.\n");
        return;
    }

    printf("\nDiagrama de Gantt:\n\n");

    int N = list->size;
    int currentTime = 0;
    Node *current = list->head;

    // Primera línea: procesos y barras verticales
    for (int i = 0; i < N; i++) {
        printf("%-8s |", current->comando);
        currentTime += current->burst;
        current = current->next;
    }
    printf("\n");

    // Segunda línea: línea temporal con marcas de tiempo
    currentTime = 0;
    current = list->head;
    printf("%-1d", currentTime);  // Tiempo inicial (0)

    for (int i = 0; i < N; i++) {
        // Imprimir espacios hasta la siguiente marca de tiempo
        for (int j = 0; j < 7; j++) {
            printf(" ");
        }
        currentTime += current->burst;
        printf("%-2d", currentTime);
        current = current->next;
    }
    printf("\n");
}
void printFCFSDetails(List *list) {
    if (isListEmpty(list)) {
        printf("No hay procesos en la lista para mostrar.\n");
        return;
    }

    int N = list->size;
    int at[N], bt[N], wt[N], tat[N], ct[N];
    Node *current = list->head;

    // Obtener burst times
    for (int i = 0; i < N; i++) {
        bt[i] = current->burst;
        at[i] = 0;  // Asumiendo que todos los procesos llegan al tiempo 0
        current = current->next;
    }

    // Calcular tiempos
    ct[0] = bt[0];
    wt[0] = 0;
    
    for (int i = 1; i < N; i++) {
        ct[i] = ct[i-1] + bt[i];
        wt[i] = ct[i-1] - at[i];
    }

    for (int i = 0; i < N; i++) {
        tat[i] = ct[i] - at[i];
    }

    // Imprimir resultados
    printf("\nPN\tAT\tBT\tCT\tWT\tTAT\n");
    printf("----------------------------------------\n");
    current = list->head;
    for (int i = 0; i < N; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\n",
               current->comando, at[i], bt[i], ct[i], wt[i], tat[i]);
        current = current->next;
    }
    
    // Imprimir promedios
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < N; i++) {
        avg_wt += wt[i];
        avg_tat += tat[i];
    }
    avg_wt /= N;
    avg_tat /= N;
    
    printf("\nPromedio Waiting Time: %.2f", avg_wt);
    printf("\nPromedio Turnaround Time: %.2f\n", avg_tat);
}