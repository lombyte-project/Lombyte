#include "types.h"
#include "rnc/sdk/kernel.h"

s32 EnableInterrupts(void) {
    enum {
        INTERRUPT_ENABLE_BIT_CLEAR = 0
    };
    u32 saved_interrupt_enable_bit;

    saved_interrupt_enable_bit = CpuReadStatus();
    saved_interrupt_enable_bit &= 0x10000;
    CpuEnableInt();
    return saved_interrupt_enable_bit != INTERRUPT_ENABLE_BIT_CLEAR;
}
