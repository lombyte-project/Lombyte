#ifndef LOMBYTE_RNC_SDK_STORAGE_FILEIO_H
#define LOMBYTE_RNC_SDK_STORAGE_FILEIO_H

#include "types.h"

void _sceFs_Rcv_Intr(s32 *);
s32  sceClose(s32);
s32  sceFsInit(void);
int  SceFsReset(void) __asm__("sceFsReset");
s32  sceRead(s32, void *, s32);
void _sceFsIobSemaMK(void);
s32  sceLseek(s32, s32, s32);
void _sceFsSemInit(void);
s32  sceWrite(s32, u8 *, s32);

#endif /* LOMBYTE_RNC_SDK_STORAGE_FILEIO_H */
