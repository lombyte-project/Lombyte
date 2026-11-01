#ifndef LOMBYTE_RNC_SDK_SCF_H
#define LOMBYTE_RNC_SDK_SCF_H

#include "types.h"

s8  *GetRomName(void);
int  IsT10K(void);
int  sceScfGetLanguage(void);
void sceScfGetLocalTimefromRTC(s32);
u8   sceScfGetSummerTime(void);
int  sceScfGetTimeZone(void);

#endif /* LOMBYTE_RNC_SDK_SCF_H */
