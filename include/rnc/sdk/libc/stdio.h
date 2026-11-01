#ifndef LOMBYTE_RNC_SDK_LIBC_STDIO_H
#define LOMBYTE_RNC_SDK_LIBC_STDIO_H

#include "rnc/sdk/library/stdio.h"
#include "types.h"

struct Reent;

int  exponent(char *, int, int);
s32  fflush(RncFile *);
s32  _fwalk(u8 *, s32 (*)(void *));
int  Printf(const char *, ...) __asm__("printf");
s32  __sfvwrite(RncFile *, RncStdioUio *);
void __sinit(struct Reent *);
void __smakebuf(RncFile *);
s32  sprintf(s8 *, const s8 *, ...);
void std();

#endif /* LOMBYTE_RNC_SDK_LIBC_STDIO_H */
