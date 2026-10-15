#include "types.h"
#include "kernel.h"

s32 EnableInterrupts(void) {
    u32 status;

    status = CpuReadStatus();
    status &= 0x10000;
    CpuEnableInt();
    return status != 0;
}
