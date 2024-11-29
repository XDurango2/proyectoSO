#include <stdio.h>
#include <stdlib.h>
#include "sjf.h"

// Ordena la lista por tiempo de ráfaga (Burst Time)
void sortListByBurst(List *list) {
    if (list->size <= 1) return;

    Node *i, *j;
    for (i = list->head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->burst > j->burst) {
                // Intercambiar procesos
                char *temp_comando = i->comando;
                int temp_burst = i->burst;
                i->comando = j->comando;
                i->burst = j->burst;
                j->comando = temp_comando;
                j->burst = temp_burst;
            }
        }
    }
}

SJFResult calculateSJF(List *list) {
    SJFResult result = {0.0, 0.0};
    if (list->size == 0) return result;

    int n = list->size;
    int wt[n];   // waiting times
    int tat[n];  // turnaround times
    int ct[n];   // completion times
    
    // Primera ordenamos por burst time
    sortListByBurst(list);
    
    // El primer proceso no espera
    wt[0] = 0;
    ct[0] = list->head->burst;
    tat[0] = ct[0];  // TAT = CT ya que AT = 0
    
    // Calcular para el resto de procesos
    Node *current = list->head->next;
    for(int i = 1; i < n; i++) {
        ct[i] = ct[i-1] + current->burst;    // Completion time
        wt[i] = ct[i-1];                     // Waiting time = CT del anterior
        tat[i] = ct[i];                      // TAT = CT ya que AT = 0
        current = current->next;
    }
    
    // Calcular promedios
    float total_wt = 0, total_tat = 0;
    for(int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
    }
    
    result.avg_waiting_time = total_wt / n;
    result.avg_turnaround_time = total_tat / n;
    return result;
}
void printSJFGantt(List *list) {
    if (isListEmpty(list)) {
        printf("No hay procesos para mostrar en el diagrama de Gantt.\n");
        return;
    }

    // Primero ordenamos la lista por burst time
    sortListByBurst(list);

    int n = list->size;
    int ct[n];   // completion times
    Node *current = list->head;

    // Calcular completion times
    ct[0] = current->burst;
    current = current->next;
    for(int i = 1; i < n; i++) {
        ct[i] = ct[i-1] + current->burst;
        current = current->next;
    }

    printf("\nDiagrama de Gantt:\n\n");

    // Primera línea: procesos y barras verticales
    current = list->head;
    for (int i = 0; i < n; i++) {
        printf("%-8s |", current->comando);
        current = current->next;
    }
    printf("\n");

    // Segunda línea: línea temporal con marcas de tiempo
    int currentTime = 0;
    current = list->head;
    printf("%-1d", currentTime);  // Tiempo inicial (0)

    for (int i = 0; i < n; i++) {
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
// Imprime los detalles de SJF
void printSJFDetails(List *list) {
    if (list->size == 0) {
        printf("No hay procesos en la lista para ejecutar SJF.\n");
        return;
    }

    int n = list->size;
    int wt[n];   // waiting times
    int tat[n];  // turnaround times
    int ct[n];   // completion times
    
    // Primera ordenamos por burst time
    sortListByBurst(list);
    
    // El primer proceso no espera
    wt[0] = 0;
    ct[0] = list->head->burst;
    tat[0] = ct[0];
    
    // Calcular para el resto de procesos
    Node *current = list->head->next;
    for(int i = 1; i < n; i++) {
        ct[i] = ct[i-1] + current->burst;
        wt[i] = ct[i-1];
        tat[i] = ct[i];
        current = current->next;
    }

    printf("\nOrden de ejecución SJF:\n");
    printf("PN\tBT\tWT\tCT\tTAT\n");
    printf("-----------------------------------\n");
    
    current = list->head;
    for(int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\n",
               current->comando,
               current->burst,
               wt[i],
               ct[i],
               tat[i]);
        current = current->next;
    }

    // Calcular y mostrar promedios
    float total_wt = 0, total_tat = 0;
    for(int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
    }
    
    printf("\nTiempo promedio de espera: %.2f", total_wt / n);
    printf("\nTiempo promedio de retorno: %.2f\n", total_tat / n);
}