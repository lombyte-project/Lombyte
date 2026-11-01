#ifndef LOMBYTE_RNC_SDK_RPC_IOP_H
#define LOMBYTE_RNC_SDK_RPC_IOP_H

#include "types.h"

s32 sceSifInitIopHeap(void);
s32 _sceSifLoadModuleBuffer(s32, s32, s32, s32 *);
s32 sceSifRebootIop(const char *);
s32 sceSifResetIop(const char *, s32);
int sceSifSyncIop(void);

#endif /* LOMBYTE_RNC_SDK_RPC_IOP_H */
