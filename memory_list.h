#ifndef MEMORY_LIST_H
#define MEMORY_LIST_H

#include <stdbool.h>

// Estructura del nodo para memoria
typedef struct memoryNode {
    int memory_size;        // Tamaño del bloque de memoria
    int process_id;         // Identificador numérico del proceso (0 para bloque libre)
    struct memoryNode *next;
} memoryNode;

// Estructura de la lista de memoria
typedef struct {
    memoryNode *head;            // Primer nodo de la lista
    memoryNode *tail;            // Último nodo de la lista
    int size;             // Número total de bloques en la lista
    int total_memory;     // Memoria total disponible
} MemoryList;

// Prototipos de funciones para manejo de memoria
void initializeMemoryList(MemoryList *list, int total_memory);
bool isMemoryListEmpty(MemoryList *list);
bool addMemoryBlock(MemoryList *list, int memory_size, int process_name);
bool removeMemoryBlock(MemoryList *list, int process_name);
memoryNode *findFreeBlock(MemoryList *list, int required_size);
void mergeFreeBlocks(MemoryList *list);
void clearMemoryList(MemoryList *list);
void printMemoryMap(MemoryList *list);

// Funciones adicionales útiles
int getTotalFreeMemory(MemoryList *list);
int getFragmentationCount(MemoryList *list);
bool compactMemory(MemoryList *list);

#endif // MEMORY_LIST_H