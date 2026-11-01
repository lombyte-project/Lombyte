#ifndef LOMBYTE_RNC_SDK_INPUT_DBC_H
#define LOMBYTE_RNC_SDK_INPUT_DBC_H

#include "types.h"

s32 sceDbcCreateSocket(void *, s32, s32);
s32 sceDbcInit(void);
s32 sceDbcSetWorkAddr(s32);
int SceDbcGetModVersion(void) __asm__("sceDbcGetModVersion");
s32 sceDbcReceiveData(s32, s32, s32 *, u8 *);

#endif /* LOMBYTE_RNC_SDK_INPUT_DBC_H */
