// rr.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rr.h"

typedef struct {
    char comando[256];
    int burst_remaining;
    int burst_original;
    int completion_time;
    int waiting_time;
    int turnaround_time;
} Process;

typedef struct {
    char comando[256];
    int start_time;
    int end_time;
} GanttItem;

// Función auxiliar para contar dígitos
static int count_digits(int num) {
    if(num == 0) return 1;
    int count = 0;
    while(num > 0) {
        count++;
        num /= 10;
    }
    return count;
}

// Función para imprimir el diagrama de Gantt
static void printGanttChart(GanttItem *gantt_sequence, int gantt_size) {
    printf("\nDiagrama de Gantt:\n");
    
    // Línea superior
    printf("┌");
    for(int i = 0; i < gantt_size; i++) {
        for(int j = 0; j < 10; j++) printf("─");
        if(i < gantt_size - 1) printf("┬");
        else printf("┐");
    }
    printf("\n");

    // Procesos
    printf("│");
    for(int i = 0; i < gantt_size; i++) {
        printf("%-9s│  ", gantt_sequence[i].comando);
    }
    printf("\n");

    // Línea inferior
    printf("└");
    for(int i = 0; i < gantt_size; i++) {
        for(int j = 0; j < 10; j++) printf("─");
        if(i < gantt_size - 1) printf("┴");
        else printf("┘");
    }
    printf("\n");

    // Tiempos
    printf("%-d", 0);
    for(int i = 0; i < gantt_size; i++) {
        int space_needed = 10 - count_digits(gantt_sequence[i].end_time);
        for(int j = 0; j < space_needed; j++) printf(" ");
        printf("%d", gantt_sequence[i].end_time);
    }
    printf("\n");
}

RRResult roundRobin(List *list, int quantum) {
    RRResult result = {0.0, 0.0};
    if (list->size == 0) return result;

    int n = list->size;
    Process *processes = malloc(n * sizeof(Process));
    GanttItem *gantt_sequence = malloc(1000 * sizeof(GanttItem)); // Espacio suficiente para la secuencia
    int gantt_index = 0;
    
    // Copiar procesos y sus burst times
    Node *current = list->head;
    for(int i = 0; i < n; i++) {
        strcpy(processes[i].comando, current->comando);
        processes[i].burst_remaining = current->burst;
        processes[i].burst_original = current->burst;
        processes[i].completion_time = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        current = current->next;
    }

    int current_time = 0;
    int remaining = n;  // Procesos pendientes

    // Ejecutar Round Robin
    while(remaining > 0) {
        int done = 1;
        
        for(int i = 0; i < n; i++) {
            if(processes[i].burst_remaining > 0) {
                done = 0;
                
                // Guardar estado para el diagrama de Gantt
                strcpy(gantt_sequence[gantt_index].comando, processes[i].comando);
                gantt_sequence[gantt_index].start_time = current_time;
                
                if(processes[i].burst_remaining > quantum) {
                    current_time += quantum;
                    processes[i].burst_remaining -= quantum;
                    gantt_sequence[gantt_index].end_time = current_time;
                }
                else {
                    current_time += processes[i].burst_remaining;
                    gantt_sequence[gantt_index].end_time = current_time;
                    processes[i].completion_time = current_time;
                    processes[i].burst_remaining = 0;
                    remaining--;

                    processes[i].turnaround_time = processes[i].completion_time;
                    processes[i].waiting_time = processes[i].turnaround_time - 
                                             processes[i].burst_original;
                }
                gantt_index++;
            }
        }
        
        if(done) break;
    }

    // Imprimir resultados
    printf("\nEjecución del algoritmo Round Robin (Quantum = %d):\n", quantum);
    printf("Proceso\tBT\tWT\tCT\tTAT\n");
    printf("--------------------------------\n");
    
    float total_wt = 0, total_tat = 0;
    for(int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\n",
               processes[i].comando,
               processes[i].burst_original,
               processes[i].waiting_time,
               processes[i].completion_time,
               processes[i].turnaround_time);
               
        total_wt += processes[i].waiting_time;
        total_tat += processes[i].turnaround_time;
    }

    printf("\nTiempo promedio de espera: %.2f", total_wt / n);
    printf("\nTiempo promedio de retorno: %.2f\n", total_tat / n);

    // Imprimir diagrama de Gantt
    printGanttChart(gantt_sequence, gantt_index);

    // Guardar resultados
    result.avg_waiting_time = total_wt / n;
    result.avg_turnaround_time = total_tat / n;

    // Liberar memoria
    free(processes);
    free(gantt_sequence);
    
    return result;
}