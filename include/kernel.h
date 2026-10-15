#ifndef LOMBYTE_KERNEL_H
#define LOMBYTE_KERNEL_H

#include "types.h"

/* Sony EE-kernel primitives: the privileged instructions C cannot express.
   This header is the only place for them; units use these macros. */
#define CpuEnableInt()  __asm__ __volatile__("ei")
#define CpuDisableInt() __asm__ __volatile__("di")
#define CpuSync()       __asm__ __volatile__("sync.l")
#define CpuSyncP()      __asm__ __volatile__("sync.p")

static inline u32 CpuReadStatus(void) {
    u32 status_register;
    __asm__ __volatile__("mfc0 %0, $12" : "=r"(status_register));
    return status_register;
}

static inline void CpuWriteStatus(u32 status_register) {
    __asm__ __volatile__("mtc0 %0, $12" : : "r"(status_register));
}

/* Kernel call made in place: number in $v1, result in $v0 (no stub). */
#define CpuSyscall(number) ({                                              \
    register s32 syscall_result __asm__("$2");                            \
    __asm__ __volatile__("li $3, %1\n\tsyscall"                            \
                         : "=r"(syscall_result) : "i"(number) : "$3");     \
    syscall_result;                                                        \
})

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
