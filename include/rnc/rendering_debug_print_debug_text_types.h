#ifndef RNC_RENDERING_DEBUG_PRINT_DEBUG_TEXT_TYPES_H
#define RNC_RENDERING_DEBUG_PRINT_DEBUG_TEXT_TYPES_H

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

#endif /* RNC_RENDERING_DEBUG_PRINT_DEBUG_TEXT_TYPES_H */
