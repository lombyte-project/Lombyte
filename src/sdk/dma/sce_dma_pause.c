#include "types.h"

extern s32 DIntr();
extern s32 EnableInterrupts();

s32 sceDmaPause(volatile u32 *chcr) {
    s32 interrupts_were_enabled;
    u32 ctrl;
    u32 chan;
    u32 dstat;
    u32 masked;

    interrupts_were_enabled = DIntr();
    ctrl = *(volatile u32 *)0x1000F520;
    if (!(ctrl & 0x10000)) {
        *(volatile u32 *)0x1000F590 = ctrl | 0x10000;
    }
    dstat = *(volatile u32 *)0x1000E000;
    chan = *chcr;
    masked = chan & 0xFFFFFEFF;
    *chcr = masked;
    *(volatile u32 *)0x1000F590 = ctrl;
    if (interrupts_were_enabled != FALSE) {
        EnableInterrupts(masked, dstat, ctrl, interrupts_were_enabled);
    }
    return chan;
}
