#ifndef RCP2P_RNC_SDK_INPUT_DBC_H
#define RCP2P_RNC_SDK_INPUT_DBC_H

#include "types.h"

s32 sceDbcCreateSocket(void *, s32, s32);
s32 sceDbcInit(void);
s32 sceDbcSetWorkAddr(s32);
int SceDbcGetModVersion(void) __asm__("sceDbcGetModVersion");
s32 sceDbcReceiveData(s32, s32, s32 *, u8 *);

#endif /* RCP2P_RNC_SDK_INPUT_DBC_H */
