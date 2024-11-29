// memory_strategies.h
#ifndef MEMORY_STRATEGY_H
#define MEMORY_STRATEGY_H

#include "memory_list.h"

typedef enum {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
} AllocationStrategy;

// Prototipos de funciones
memoryNode *findFreeBlockStrategy(MemoryList *list, int required_size, AllocationStrategy strategy);
bool addMemoryBlockStrategy(MemoryList *list, int memory_size, int process_id, AllocationStrategy strategy);

#endif // MEMORY_STRATEGY_H