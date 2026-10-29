/* _clearEach c2: reset IPU/GS state registers and sync. */

#include "types.h"

extern s32 DIntr();
extern void EnableInterrupts();
extern s32 sceIpuSync();

void _clearEach(u8 *ctx) {
    s32 state;

    *(s32 *)(ctx + 0x818) = 1;
    *(s32 *)(ctx + 0x1B0) = 0;
    state = DIntr();

    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B000 = 0;
    *(volatile u32 *)0x1000B400 = 0;
    *(volatile u32 *)0x1000D400 = 0;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & 0xFFFEFFFF;

    if (state != 0)
        EnableInterrupts();

    *(volatile u32 *)0x1000B020 = 0;
    *(volatile u32 *)0x1000B420 = 0;
    *(volatile u32 *)0x1000D420 = 0;
    *(volatile u32 *)0x10002010 = 0x40000000;

    sceIpuSync(0, 0);
}
