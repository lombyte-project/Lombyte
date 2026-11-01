#ifndef LOMBYTE_RNC_SDK_STORAGE_CDVD_H
#define LOMBYTE_RNC_SDK_STORAGE_CDVD_H

#include "types.h"

struct sceCdRMode;

s32  PowerOffCB(void);
void cdvd_exit(void);
s32  checkModelVersion(void);
void cmd_sem_init(void);
void handle_cd_read_callback(s32 *) __asm__("FUN_001206d8");
s32  sceCdCallback(s32);
void SceCdDelayThread(int) __asm__("sceCdDelayThread");
s32  sceCdDiskReady(s32) __asm__("FUN_00121190");
s32  sceCdGetError(void);
int  sceCdInit(int);
s32  sceCdNcmdDiskReady(void);
s32  sceCdRead(u32, u32, void *, struct sceCdRMode *);
s32  sceCdSyncS(s32);
s32  sceCdBreak(void);
s32  sceCdMmode(s32);
s32  sceCdReadClock(u64 *);
s32  sceCdSync(s32);
void signal_native_cd_delay_alarm(s32, u32, s32) __asm__("FUN_001205e8");

#endif /* LOMBYTE_RNC_SDK_STORAGE_CDVD_H */
