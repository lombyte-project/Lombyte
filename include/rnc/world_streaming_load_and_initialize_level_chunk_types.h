#ifndef RNC_WORLD_STREAMING_LOAD_AND_INITIALIZE_LEVEL_CHUNK_TYPES_H
#define RNC_WORLD_STREAMING_LOAD_AND_INITIALIZE_LEVEL_CHUNK_TYPES_H

#include "types.h"

typedef struct {
    int status;
    void *func;
    void *stack;
    int stackSize;
    void *gpReg;
    int initPriority;
    int currentPriority;
    int attr;
    int option;
} ThreadParam;

#endif /* RNC_WORLD_STREAMING_LOAD_AND_INITIALIZE_LEVEL_CHUNK_TYPES_H */
