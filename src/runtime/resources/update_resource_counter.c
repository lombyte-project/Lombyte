#include "types.h"

s32 ResourceCounter __asm__("D_0015F8F8") = 0;

void DecrementResourceCounter(void) __asm__("UpdateResourceCounter");

void DecrementResourceCounter(void) {
    if (ResourceCounter != 0) {
        ResourceCounter -= 1;
    }
}
