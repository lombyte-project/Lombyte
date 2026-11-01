#ifndef LOMBYTE_RNC_RUNTIME_DECREMENT_RESOURCE_COUNTER_H
#define LOMBYTE_RNC_RUNTIME_DECREMENT_RESOURCE_COUNTER_H

#include "types.h"

void decrement_resource_counter(void) __asm__("UpdateResourceCounter");

#endif /* LOMBYTE_RNC_RUNTIME_DECREMENT_RESOURCE_COUNTER_H */
