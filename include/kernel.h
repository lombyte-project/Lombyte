#ifndef LOMBYTE_KERNEL_H
#define LOMBYTE_KERNEL_H

#include "types.h"

/* Sony EE-kernel interrupt primitives. */
#define CpuEnableInt()  __asm__ __volatile__("ei")
#define CpuDisableInt() __asm__ __volatile__("di")

static inline u32 CpuReadStatus(void) {
    u32 status_register;
    __asm__ __volatile__("mfc0 %0, $12" : "=r"(status_register));
    return status_register;
}

static inline void CpuWriteStatus(u32 status_register) {
    __asm__ __volatile__("mtc0 %0, $12" : : "r"(status_register));
}

/* Semaphore parameters of CreateSema/ReferSemaStatus (SDK eekernel.h). */
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    u32 attr;
    u32 option;
};

#endif /* LOMBYTE_KERNEL_H */
