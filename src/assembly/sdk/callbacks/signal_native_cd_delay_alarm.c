#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/sdk/callbacks/signal_native_cd_delay_alarm/FUN_001205e8.s",
    FUN_001205e8);
#else
#include "types.h"

extern s32 iSignalSema(s32 semaphore_id);
extern void __sync_synchronize(void);
extern void CpuEnableInt(void);

s32 signal_native_cd_delay_alarm(s32 alarm_id, u32 scheduled_time, s32 semaphore_id)
    __asm__("FUN_001205e8");

s32 signal_native_cd_delay_alarm(s32 alarm_id, u32 scheduled_time, s32 semaphore_id) {
    s32 result = iSignalSema(semaphore_id);
    volatile s32 *saved = &result;
    __sync_synchronize();
    CpuEnableInt();
    return *saved;
}
#endif /* NON_MATCHING */
