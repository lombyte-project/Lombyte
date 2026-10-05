#include "types.h"
#include "asm.h"

#include "kernel.h"

extern s32 iSignalSema(s32 nSemaphore);

s32 signal_native_cd_delay_alarm(s32 nAlarmId, u32 dwScheduledTime, s32 nSemaphore) __asm__("FUN_001205e8");

s32 signal_native_cd_delay_alarm(s32 nAlarmId, u32 dwScheduledTime, s32 nSemaphore) {
    s32 result = iSignalSema(nSemaphore);

    /* The retail callback completes the signal before synchronizing and enabling interrupts. */
    __asm__ __volatile__("sync");
    CpuEnableInt();
    return result;
}

extern __typeof__(signal_native_cd_delay_alarm) D_001205E8 __attribute__((alias("FUN_001205e8")));
extern __typeof__(signal_native_cd_delay_alarm) func_001205E8 __attribute__((alias("FUN_001205e8")));
