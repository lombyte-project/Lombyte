#include "types.h"

s32 ResourceCounter __asm__("D_0015F8F8") = 0;

void decrement_resource_counter(void) __asm__("UpdateResourceCounter");

void decrement_resource_counter(void) {
    if (ResourceCounter != 0) {
        ResourceCounter -= 1;
    }
}
