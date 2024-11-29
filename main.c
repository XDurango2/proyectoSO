#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "list.h"
#include "fcfs.h"
#include "sjf.h"
#include "rr.h"
#include "memory_list.h"
#include "memory_strategy.h"
#define BUFFER_SIZE 1024
#define MAX_ARGS 100


List l;
MemoryList memory_list;

// Función auxiliar para verificar si todos los procesos tienen memoria asignada
bool all_processes_have_memory(List *list) {
    Node *current = list->head;
    while (current != NULL) {
        if (!current->memory_allocated) {
            printf("El proceso %d no tiene memoria asignada\n", current->pid);
            return false;
        }
        current = current->next;
    }
    return true;
}

void memory_sim_creation(){
    initializeMemoryList(&memory_list,BUFFER_SIZE);
    printf("servicios de memoria iniciados \n");
    printf("memoria disponible %d bloques",getTotalFreeMemory(&memory_list));
}
void memory_sim_view(){
    printMemoryMap(&memory_list);
}

void freeman(int pid){
    printf("eliminacion de la asignacion de memoria del proceso\n");
    //Node *node = findProcessByPID(&l,pid);
    if(removeMemoryBlock(&memory_list,pid)==true){
        //node->memory_allocated=false;
        printf("elimiacion exitosa");

    }else{
        printf("error en la eliminiacion de la asignacion de memoria del proceso");
    }
}
void psview(){
    printList(&l);
}
void killer(int pid){
    if(removeMemoryBlock(&memory_list,pid)==true){
    removeFromList(&l,pid);
    printf("eliminacion total del proceso \n");
    printList(&l);
    memory_sim_view();

    }

}

bool memory_worst_fit(int size,int process_id){
    if (addMemoryBlockStrategy(&memory_list, size, process_id, WORST_FIT)) {
        printf("worst Fit exitoso para el proceso ID: %d\n", process_id);
        return true;
    } else {
        printf("Error: No se pudo asignar memoria usando First Fit\n");
        return false;
    }
}

bool memory_first_fit(int size, int process_id) {
    if (addMemoryBlockStrategy(&memory_list, size, process_id, FIRST_FIT)) {
        printf("First Fit exitoso para el proceso ID: %d\n", process_id);
        return true;
    } else {
        printf("Error: No se pudo asignar memoria usando First Fit\n");
        return false;
    }
}

bool memory_best_fit(int size,int process_id){
    if (addMemoryBlockStrategy(&memory_list, size, process_id, BEST_FIT)) {
        printf("best Fit exitoso para el proceso ID: %d\n", process_id);
        return true;
    } else {
        printf("Error: No se pudo asignar memoria usando First Fit\n");
        return false;
    }
   

}

void memprocess(List *list, int pid, char AllocStrat) {
    Node *process = findProcessByPID(list, pid);
    if (process == NULL) {
        printf("Proceso con PID %d no encontrado\n", pid);
        return;
    }

    if (process->memory_allocated) {
        printf("Proceso ya tiene memoria asignada\n");
        return;
    }

    bool allocation_success = false;
    switch(AllocStrat) {
        case 'W':
            allocation_success = memory_worst_fit(process->memSize, pid);
            break;
        case 'B':
            allocation_success = memory_best_fit(process->memSize, pid);
            break;
        case 'F':
            allocation_success = memory_first_fit(process->memSize, pid);
            break;
        default:
            printf("Estrategia de asignación no válida\n");
            return;
    }

    if (allocation_success) {
        process->memory_allocated = true;
        printf("Memoria asignada al proceso %d\n", pid);
    } else {
        printf("No se pudo asignar memoria al proceso %d\n", pid);
    }
}

void mkprocess(char* comando,int pid,int burst,int memSize){
    if(addToList(&l,comando,pid,burst,memSize)==true){
            printf("Proceso creado y dentro de la ready queue: %s (Burst: %d)\n", comando, burst);

    }
}

void rr(int quantum) {
    if (isListEmpty(&l)) {
        printf("No hay procesos en la lista para ejecutar Round Robin.\n");
        return;
    }

    // Verificar si todos los procesos tienen memoria asignada
    if (!all_processes_have_memory(&l)) {
        printf("No se puede ejecutar Round Robin. Algunos procesos no tienen memoria asignada.\n");
        return;
    }

    roundRobin(&l, quantum);
    clearList(&l);
   // clearMemoryList(&memory_list);
    //initializeMemoryList(&memory_list,BUFFER_SIZE);

}

void fcfs() {
    if (isListEmpty(&l)) {
        printf("No hay procesos en la lista para ejecutar FCFS.\n");
        return;
    }

    // Verificar si todos los procesos tienen memoria asignada
    if (!all_processes_have_memory(&l)) {
        printf("No se puede ejecutar FCFS. Algunos procesos no tienen memoria asignada.\n");
        return;
    }

    printf("\nEjecución del algoritmo FCFS:\n");
    printFCFSDetails(&l);
    printFCFSGantt(&l);
    FCFSResult result = calculateFCFS(&l);
    clearList(&l);
    //clearMemoryList(&memory_list);
    //initializeMemoryList(&memory_list,BUFFER_SIZE);

}

// Comando para ejecutar SJF
void sjf() {
    if (isListEmpty(&l)) {
        printf("No hay procesos en la lista para ejecutar SJF.\n");
        return;
    }

    // Verificar si todos los procesos tienen memoria asignada
    if (!all_processes_have_memory(&l)) {
        printf("No se puede ejecutar SJF. Algunos procesos no tienen memoria asignada.\n");
        return;
    }

    sortListByBurst(&l);  // Ordenar por burst
    printSJFDetails(&l);  // Imprimir detalles
    printFCFSGantt(&l);
    clearList(&l);
    //clearMemoryList(&memory_list);
    //initializeMemoryList(&memory_list,BUFFER_SIZE);

}

void ejecutar(char *comando) {
    char *args[MAX_ARGS];
    int k = 0;
    args[k] = strtok(comando, " ");
    while (args[k] != NULL && k < MAX_ARGS - 1) {
        k++;
        args[k] = strtok(NULL, " ");
    }
    args[k] = NULL;
    
    execvp(args[0], args);
    exit(EXIT_FAILURE);
}

void create_pipes(char *comando){
    char *subcomando;
    int fd[2];
    int entrada =0;
    subcomando = strtok(comando,"|");
    while(subcomando!=NULL){
        pipe(fd);
        if(fork()==0){
            dup2(entrada,STDIN_FILENO);
            if(strtok(NULL,"|")!=NULL){
                dup2(fd[1],STDOUT_FILENO);
            }
            close(fd[0]);
            ejecutar(subcomando);
        }else{
            wait(NULL);
            close(fd[1]);


            entrada=fd[0];
            subcomando =strtok(NULL,"|");
        }
    }
}


int main() {
    initializeList(&l);
    char comando[BUFFER_SIZE];

    while (1) {
        printf("\nComando> ");
        fgets(comando, BUFFER_SIZE, stdin);
        comando[strcspn(comando, "\n")] = 0;

        if (strcmp(comando, "exit") == 0) {
            printf("Saliendo, espere...\n");
            break;
        }
        // Comando "memprocess"
        if (strncmp(comando, "memprocess", 9) == 0) {
            int pid;
            char AllocStrat;
            if (sscanf(comando + 10, "%d %c", &pid,&AllocStrat) == 2) {
                memprocess(&l,pid, AllocStrat);
            } else {
                printf("Uso: memProcess <pid> <AllocStrat>\n");
            }
            continue;
        }
        // Comando "mkprocess"
        if (strncmp(comando, "mkprocess", 9) == 0) {
            char proceso[BUFFER_SIZE];
            int burst;
            int memSize;
            int pid;
            if (sscanf(comando + 10, "%s %d %d %d", proceso, &pid,&burst,&memSize) == 4) {
                mkprocess(proceso, pid,burst, memSize);
            } else {
                printf("Uso: mkprocess <proceso> <pid> <burst> <memSize>\n");
            }
            continue;
        }

        // Comando "psview"
        if (strcmp(comando, "psview") == 0) {
            psview();
            continue;
        }

        // Comando "fcfs"
        if (strcmp(comando, "fcfs") == 0) {
            fcfs();
            continue;
        }
        // Comando "sjf"
        if (strcmp(comando, "sjf") == 0) {
            sjf();
            continue;
        }
        // Comando "rr"
        if (strncmp(comando, "rr", 2) == 0) {
            int quantum;
            if (sscanf(comando + 3, "%d", &quantum) == 1) {
                rr(quantum);
            } else {
                printf("Uso: rr <quantum>\n");
            }
            continue;
        }
       
        if(strcmp(comando,"memstart")== 0){
            memory_sim_creation();
            continue;
        }
        
        if(strcmp(comando,"memview")==0){
            memory_sim_view();
            continue;
        }
        
        if(strcmp(comando,"memcompact")== 0){
            if(compactMemory(&memory_list)==true){
                printf("compactacion de memoria exitosa");
            }else{
                printf("error en la compactacion de memoria");
            }
            continue;
        }
        // Comando "freeman"
        if (strncmp(comando, "freeman", 6) == 0) {
            int pid;
            if (sscanf(comando + 7, "%d", &pid) == 1) {
                freeman(pid);
            } else {
                printf("Uso: freeman <pid>\n");
            }
            continue;
        }

        // Comando "killer"
        if (strncmp(comando, "killer", 6) == 0) { // Comparar los primeros 6 caracteres
            int pid;
            if (sscanf(comando + 7, "%d", &pid) == 1) { // Leer desde el índice correcto
                killer(pid);
            } else {
                printf("Uso: killer <pid>\n");
            }
            continue;
        }
        

        
        // Otros comandos
        create_pipes(comando);
    }

    clearList(&l);
    clearMemoryList(&memory_list);
    return 0;
}

