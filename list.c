#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Asegúrate de incluir esta biblioteca
#include <stdio.h>

// Inicializa la lista
void initializeList(List *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Verifica si la lista está vacía
bool isListEmpty(List *list) {
    return list->size == 0;
}
Node *findProcessByPID(List *list, int pid) {
    Node *current = list->head;
    while (current != NULL) {
        if (current->pid == pid) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

bool addToList(List *list, char *comando, int pid, int burst, int memSize) {
    // Verificar si el PID ya existe en la lista
    Node *current = list->head;
    while (current != NULL) {
        if (current->pid == pid) {
            printf("Error: Ya existe un proceso con el PID %d en la lista.\n", pid);
            return false; // No agrega el nodo
        }
        current = current->next;
    }

    // Crear un nuevo nodo
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        perror("Error al asignar memoria para el nodo");
        exit(EXIT_FAILURE);
    }

    newNode->comando = strdup(comando);
    newNode->burst = burst;
    newNode->memSize = memSize;
    newNode->pid = pid;
    newNode->next = NULL;

    // Agregar el nodo a la lista
    if (isListEmpty(list)) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }

    list->size++;
    printf("Proceso con PID %d agregado a la lista.\n", pid);
    return true;
}


Node *removeFromList (List *list, int pid) {
    if (isListEmpty(list)) {
        return NULL;
    }

    Node *current = list->head;
    Node *previous = NULL;

    // Recorrer la lista para encontrar el nodo con el PID deseado
    while (current != NULL) {
        if (current->pid == pid) {
            // Nodo encontrado, actualizar enlaces
            if (previous == NULL) {
                // El nodo a eliminar es el primero
                list->head = current->next;
            } else {
                // Saltar el nodo actual
                previous->next = current->next;
            }

            // Si el nodo a eliminar es el último, actualizar la cola
            if (current == list->tail) {
                list->tail = previous;
            }

            list->size--; // Reducir el tamaño de la lista

            return current; // Devolver el nodo eliminado
        }

        // Avanzar al siguiente nodo
        previous = current;
        current = current->next;
    }

    // Si llegamos aquí, el nodo no fue encontrado
    return NULL;
}

// Devuelve el primer nodo de la lista sin removerlo
Node *peekList(List *list) {
    if (isListEmpty(list)) {
        return NULL;
    }
    return list->head;
}

// Libera toda la memoria de la lista
void clearList(List *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp->comando); // Liberar memoria del comando
        free(temp);          // Liberar el nodo
    }

    list->head = NULL; // La lista ahora está vacía
    list->tail = NULL;
    list->size = 0;
}

// Add this function to list.c
Node *findAndRemoveProcess(List *list, int pid) {
    if (isListEmpty(list)) {
        return NULL;
    }

    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (current->pid == pid) {
            // If the node to remove is the head
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }

            // Update tail if removing the last node
            if (current == list->tail) {
                list->tail = previous;
            }

            list->size--;
            return current;
        }

        previous = current;
        current = current->next;
    }

    return NULL;  // Process not found
}
void printList(List *list) {
    if (isListEmpty(list)) {
        printf("La lista está vacía.\n");
        return;
    }

    Node *current = list->head;
    printf("Contenido de la lista:\n");

    while (current != NULL) {
        printf("Comando: %s, PID: %d, Burst: %d, MemSize: %d, Memoria asignada: %s\n",
               current->comando,
               current->pid,
               current->burst,
               current->memSize,
               current->memory_allocated ? "Sí" : "No");
        current = current->next;
    }
}
