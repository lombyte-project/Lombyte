#ifndef LOMBYTE_RNC_SDK_KERNEL_H
#define LOMBYTE_RNC_SDK_KERNEL_H

#include "types.h"
#include "rnc/sdk/top_thread.h"
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
#define CpuSyscall(number)                                                                        \
    ({                                                                                            \
        register s32 syscall_result __asm__("$2");                                                \
        __asm__ __volatile__("li $3, %1\n\tsyscall" : "=r"(syscall_result) : "i"(number) : "$3"); \
        syscall_result;                                                                           \
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

void  InitExecPS2(void);
void  InitSystemCallTableAddress(void);
s32   InitThread(void);
s32   PatchIsNeeded(void);
s64   VSync2(void);
void  clear_tty_initialized(void) __asm__("func_00118BC0");
s32   iWakeupThread(s32);
void  InitAlarm(void);
s32   InitTLB(void);
void  InitTLBFunctions(void);
s32   read(s32, s32, s32);
void *sbrk(s32);
void  SupplementCrt0(void) __asm__("supplement_crt0");
void  topThread(TopThreadQueue *);
void  VSync(void);
s32   write(s32, s32, s32);

#endif /* LOMBYTE_RNC_SDK_KERNEL_H */
