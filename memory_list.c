#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_list.h"

void initializeMemoryList(MemoryList *list, int total_memory) {
    // If total_memory is 0, use the existing total_memory
    if (total_memory <= 0) {
        total_memory = list->total_memory;
    }

    // Free existing nodes if any
    clearMemoryList(list);

    // Allocate new head node
    list->head = malloc(sizeof(memoryNode));
    if (list->head == NULL) {
        printf("Error: No se pudo inicializar la lista de memoria\n");
        return;
    }

    // Set the initial block as a free block
    list->head->memory_size = total_memory;
    list->head->process_id = 0;  // 0 indicates free block
    list->head->next = NULL;

    list->tail = list->head;
    list->size = 1;
    
    // Preserve the total memory
    list->total_memory = total_memory;
}

bool isMemoryListEmpty(MemoryList *list) {
    return list->size == 0;
}

memoryNode *findFreeBlock(MemoryList *list, int required_size) {
    memoryNode *current = list->head;

    while (current != NULL) {
        if (current->process_id == 0 && current->memory_size >= required_size) {
            return current;
        }
        current = current->next;
    }

    return NULL;  // No se encontró un bloque adecuado
}


bool removeMemoryBlock(MemoryList *list, int process_id) {
    memoryNode *current = list->head;

    while (current != NULL) {
        if (current->process_id == process_id) {
            // Liberar el bloque
            current->process_id = 0;

            // Intentar fusionar bloques adyacentes libres
            //mergeFreeBlocks(list);
            return true;
        }
        current = current->next;
    }

    printf("Error: Proceso con ID %d no encontrado\n", process_id);
    return false;
}


void clearMemoryList(MemoryList *list) {
    memoryNode *current = list->head;

    while (current != NULL) {
        memoryNode *temp = current;
        current = current->next;

        free(temp);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->total_memory = 0;
}

// Updated memory_list.c

int getTotalFreeMemory(MemoryList *list) {
    int free_memory = 0;
    memoryNode *current = list->head;
    
    while (current != NULL) {
        if (current->process_id == 0) {  // Changed from NULL to 0
            free_memory += current->memory_size;
        }
        current = current->next;
    }
    
    return free_memory;
}

void mergeFreeBlocks(MemoryList *list) {
    memoryNode *current = list->head;

    while (current != NULL && current->next != NULL) {
        if (current->process_id == 0 && current->next->process_id == 0) {
            memoryNode *temp = current->next;
            current->memory_size += temp->memory_size;
            current->next = temp->next;

            if (temp == list->tail) {
                list->tail = current;
            }

            free(temp);
        } else {
            current = current->next;
        }
    }
}


int getFragmentationCount(MemoryList *list) {
    int count = 0;
    memoryNode *current = list->head;
    
    while (current != NULL) {
        if (current->process_id == 0) {  // Changed from NULL to 0
            count++;
        }
        current = current->next;
    }
    
    return count;
}

bool compactMemory(MemoryList *list) {
    printf("Starting memory compaction...\n");
    int total_memory = list->total_memory;

    // If total memory is 0 or list is empty, return false
    if (list->total_memory <= 0 || list->head == NULL) {
        printf("Error: Invalid memory list state\n");
        return false;
    }
    
    // Create a temporary list to store occupied blocks
    memoryNode *temp_head = NULL;
    memoryNode *temp_tail = NULL;
    int total_used = 0;
    
    // Move all occupied blocks to the temporary list
    memoryNode *current = list->head;
    while (current != NULL) {
        if (current->process_id != 0) {
            printf("Found occupied block: size %d, process ID %d\n", 
                   current->memory_size, current->process_id);
            
            memoryNode *new_node = malloc(sizeof(memoryNode));
            if (new_node == NULL) {
                printf("Error: Could not allocate memory for compaction\n");
                return false;
            }
            
            new_node->memory_size = current->memory_size;
            new_node->process_id = current->process_id;
            new_node->next = NULL;
            
            total_used += current->memory_size;
            
            if (temp_head == NULL) {
                temp_head = new_node;
                temp_tail = new_node;
            } else {
                temp_tail->next = new_node;
                temp_tail = new_node;
            }
        }
        current = current->next;
    }
    
    printf("Total used memory in occupied blocks: %d\n", total_used);
    total_memory=total_memory-total_used;
    // If no occupied blocks, just reset the list
    if (total_used == 0) {
        clearMemoryList(list);
        initializeMemoryList(list, list->total_memory);
        return true;
    }
    
    // Verify that total used memory doesn't exceed total memory
    if (total_used > list->total_memory) {
        printf("Error: Used memory exceeds total memory\n");
        return false;
    }
    
    // Clear the original list completely
    clearMemoryList(list);
    printf("Original memory list cleared\n");
    
    initializeMemoryList(list, total_memory);
    printf("Reinitialized memory list with total memory %d\n", total_memory);
    
    // Add back the occupied blocks
    current = temp_head;
    while (current != NULL) {
        memoryNode *next = current->next;
        
        printf("Attempting to add block: size %d, process ID %d\n", 
               current->memory_size, current->process_id);
        
        // Create the block directly instead of using addMemoryBlock
        memoryNode *new_block = malloc(sizeof(memoryNode));
        if (new_block == NULL) {
            printf("Error: Could not allocate memory block\n");
            // Clean up
            while (temp_head) {
                memoryNode *temp = temp_head;
                temp_head = temp_head->next;
                free(temp);
            }
            return false;
        }
        
        new_block->memory_size = current->memory_size;
        new_block->process_id = current->process_id;
        
        // Find the tail and append
        if (list->head == NULL) {
            list->head = new_block;
            list->tail = new_block;
            new_block->next = NULL;
        } else {
            list->tail->next = new_block;
            list->tail = new_block;
            new_block->next = NULL;
        }
        
        list->size++;
        
        free(current);
        current = next;
    }
    
    // Merge any free blocks
    mergeFreeBlocks(list);
    
    printf("Memory compaction completed successfully\n");
    return true;
}

// Add this function to your memory_list.c
bool addMemoryBlock(MemoryList *list, int memory_size, int process_id) {
    memoryNode *free_block = findFreeBlock(list, memory_size);

    if (free_block == NULL) {
        printf("Error: No hay suficiente memoria contigua disponible\n");
        return false;
    }

    if (free_block->memory_size > memory_size) {
        memoryNode *new_node = malloc(sizeof(memoryNode));
        if (new_node == NULL) {
            printf("Error: No se pudo asignar memoria para el nuevo nodo\n");
            return false;
        }

        new_node->memory_size = free_block->memory_size - memory_size;
        new_node->process_id = 0;  // Libre
        new_node->next = free_block->next;

        free_block->memory_size = memory_size;
        free_block->next = new_node;

        if (free_block == list->tail) {
            list->tail = new_node;
        }
    }

    free_block->process_id = process_id;  // Asigna el ID del proceso
    return true;
}
void printMemoryMap(MemoryList *list) {
    printf("\nMapa de Memoria:\n");
    printf("----------------\n");

    memoryNode *current = list->head;
    int position = 0;

    while (current != NULL) {
        printf("Posición: %d - %d | ", position, position + current->memory_size - 1);
        if (current->process_id == 0) {
            printf("LIBRE (%d bytes)\n", current->memory_size);
        } else {
            printf("Proceso ID: %d (%d bytes)\n", current->process_id, current->memory_size);
        }

        position += current->memory_size;
        current = current->next;
    }

    printf("\nMemoria total: %d bytes\n", list->total_memory);
    printf("Fragmentos libres: %d\n", getFragmentationCount(list));
    printf("----------------\n");
}

