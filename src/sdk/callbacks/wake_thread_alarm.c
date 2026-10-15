#include "types.h"
#include "kernel.h"

extern s32 iWakeupThread(s32 thread_id);

/* Alarm handler: wakes the waiting thread, then re-enables interrupts. */
void wake_thread_alarm(s32 alarm_id, u32 scheduled_time, s32 thread_id) __asm__("FUN_00123bc0");

void wake_thread_alarm(s32 alarm_id, u32 scheduled_time, s32 thread_id) {
    iWakeupThread(thread_id);
    CpuSync();
    CpuEnableInt();
}

extern __typeof__(wake_thread_alarm) func_00123BC0 __attribute__((alias("FUN_00123bc0")));
