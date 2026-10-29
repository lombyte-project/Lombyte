#include "types.h"
#include "kernel.h"

extern s32 iSignalSema(s32 semaphore_id);

/* Alarm handler: wakes the CD delay waiter, then re-enables interrupts. */
void signal_native_cd_delay_alarm(s32 alarm_id, u32 scheduled_time, s32 semaphore_id)
    __asm__("FUN_001205e8");

void signal_native_cd_delay_alarm(s32 alarm_id, u32 scheduled_time, s32 semaphore_id) {
    iSignalSema(semaphore_id);
    CpuSync();
    CpuEnableInt();
}

extern __typeof__(signal_native_cd_delay_alarm) func_001205E8 __attribute__((alias("FUN_001205e8")));
