#ifndef LOMBYTE_RNC_SDK_DECI2_H
#define LOMBYTE_RNC_SDK_DECI2_H

#include "types.h"

int  CallDebugCharacter(int, int);
void deci2Putchar(s32);
int  kprintf(const char *, ...);
s32  kputchar(s32);
s32  Kputs(s8 *);
void printfloat(f64);
s32  SceDeci2ExRecv(s32, void *, u16);
s32  SceDeci2ExSend(s32, void *, u16);
s32  sceDeci2Open();
void SceDeci2Poll(s32);
int  scePrintf(const char *, ...);
void sceTtyHandler();
s32  sceTtyInit(void);
s32  sceTtyRead(s8 *, s32);
s32  sceTtyWrite(s8 *, s32);

#endif /* LOMBYTE_RNC_SDK_DECI2_H */
