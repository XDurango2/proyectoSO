#ifndef FCFS_H
#define FCFS_H

#include "list.h" // Incluir la cabecera de la lista para manejar los procesos

// Estructura para almacenar resultados de FCFS
typedef struct {
    int *wt;  // Tiempos de espera
    int *tat; // Tiempos de retorno
    int *bt;  // Tiempos de ráfaga
    int *at;  // Tiempos de llegada
    int size; // Número de procesos
    float avg_wt;  // Promedio de tiempos de espera
    float avg_tat; // Promedio de tiempos de retorno
} FCFSResult;

// Prototipos de funciones

/**
 * Inicializa las estructuras necesarias para FCFS.
 * @param list Puntero a la lista de procesos.
 */
void initializeFCFS(List *list);

/**
 * Calcula los tiempos de espera y retorno utilizando el algoritmo FCFS.
 * @param list Puntero a la lista de procesos.
 * @return Estructura con los tiempos promedios calculados.
 */
FCFSResult calculateFCFS(List *list);

void printFCFSGantt(List *list);

/**
 * Imprime los tiempos de llegada, ráfagas, espera y retorno de los procesos en la lista.
 * @param list Puntero a la lista de procesos.
 */
void printFCFSDetails(List *list);

#endif // FCFS_H
