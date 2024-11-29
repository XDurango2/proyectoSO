#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

// Nodo de la lista
typedef struct Node {
    char *comando;
    int pid;
    int burst;
    int memSize;         // Dato almacenado 
    struct Node *next;  // Puntero al siguiente nodo
    bool memory_allocated; 
} Node;

// Estructura de la lista
typedef struct {
    Node *head;  // Puntero al primer nodo
    Node *tail;  // Puntero al último nodo
    int size;    // Tamaño de la lista
} List;

// Prototipos de funciones
void initializeList(List *list);
bool isListEmpty(List *list);
bool addToList(List *list, char *comando,int pid,int burst,int memSize);
Node *peekList(List *list);
void clearList(List *list);
Node *removeFromList(List *list, int pid);
Node *findAndRemoveProcess(List *list, int pid);
Node *findProcessByPID(List *list, int pid);
void printList(List *list);
#endif // LIST_H
