#ifndef LOMBYTE_RNC_SDK_STORAGE_MEMORY_CARD_H
#define LOMBYTE_RNC_SDK_STORAGE_MEMORY_CARD_H

#include "types.h"

void mcDelayThread(int);
void mceStorePwd(char *);
s32  sceMcChdir(s32, s32, s8 *, s32);
s32  sceMcClose(s32);
s32  sceMcDelete(s32, s32, char *);
s32  SceMcFormat(s32, s32);
s32  sceMcGetDir(s32, s32, s8 *, s32, s32, s32);
s32  sceMcGetInfo(s32, s32, s32, s32, s32);
s32  sceMcInit(void);
s32  sceMcMkdir(s32, s32, const char *);
s32  sceMcOpen(s32, s32, char *, s32);
s32  sceMcRead(s32, s32 *, s32);
s32  sceMcSeek(s32, s32, s32);
s32  sceMcSync(s32, s32 *, s32 *);
s32  SceMcUnformat(s32, s32);
s32  sceMcWrite(s32, u8 *, s32);

#endif /* LOMBYTE_RNC_SDK_STORAGE_MEMORY_CARD_H */
