#include "types.h"
#include "rnc/sdk/kernel.h"

s32 EnableInterrupts(void) {
    enum {
        INTERRUPT_ENABLE_BIT_CLEAR = 0
    };
    u32 status;

    status = CpuReadStatus();
    status &= 0x10000;
    CpuEnableInt();
    return status != INTERRUPT_ENABLE_BIT_CLEAR;
}
