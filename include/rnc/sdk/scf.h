#ifndef RCP2P_RNC_SDK_SCF_H
#define RCP2P_RNC_SDK_SCF_H

#include "types.h"

s8  *GetRomName(void);
int  IsT10K(void);
int  sceScfGetLanguage(void);
void sceScfGetLocalTimefromRTC(s32);
u8   sceScfGetSummerTime(void);
int  sceScfGetTimeZone(void);

#endif /* RCP2P_RNC_SDK_SCF_H */
