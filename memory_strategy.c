// memory_strategies.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "memory_strategy.h"
#include "memory_list.h"

// Updated memory_strategy.c
memoryNode *findFreeBlockStrategy(MemoryList *list, int required_size, AllocationStrategy strategy) {
    memoryNode *current = list->head;
    memoryNode *selected_block = NULL;
    
    switch(strategy) {
        case FIRST_FIT:
            while (current != NULL) {
                if (current->process_id == 0 && current->memory_size >= required_size) {  // Changed from NULL to 0
                    return current;
                }
                current = current->next;
            }
            break;
            
        case BEST_FIT:
            {
                int smallest_difference = INT_MAX;
                
                while (current != NULL) {
                    if (current->process_id == 0 && current->memory_size >= required_size) {  // Changed from NULL to 0
                        int difference = current->memory_size - required_size;
                        if (difference < smallest_difference) {
                            smallest_difference = difference;
                            selected_block = current;
                        }
                    }
                    current = current->next;
                }
            }
            break;
            
        case WORST_FIT:
            {
                int largest_size = -1;
                
                while (current != NULL) {
                    if (current->process_id == 0 && current->memory_size >= required_size) {  // Changed from NULL to 0
                        if (current->memory_size > largest_size) {
                            largest_size = current->memory_size;
                            selected_block = current;
                        }
                    }
                    current = current->next;
                }
            }
            break;
    }
    
    return selected_block;
}
bool addMemoryBlockStrategy(MemoryList *list, int memory_size, int process_id, AllocationStrategy strategy) {
    memoryNode *free_block = findFreeBlockStrategy(list, memory_size, strategy);

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
