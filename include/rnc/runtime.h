#ifndef RCP2P_RNC_RUNTIME_H
#define RCP2P_RNC_RUNTIME_H

#include "types.h"

void decrement_resource_counter(void) __asm__("UpdateResourceCounter");

#endif /* RCP2P_RNC_RUNTIME_H */
