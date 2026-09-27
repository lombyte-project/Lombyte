#include "types.h"

struct DmaTag {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

/* The VU1 packet pointer: a one-field struct so every store through .p
   reloads the global, exactly like retail. */
struct TagPtr {
    struct DmaTag *p;
};

/* The DMA-busy mask this file owns: both functions below read it
   gp-relative, which needs its definition ahead of them. */
volatile s32 D_00160EE0 = 0;
extern s32 D_00160EF8[];
extern struct TagPtr D_00160F00;
extern s32 D_00160F0C;
extern s32 D_00160F10;
extern s32 D_00160F14;
extern char D_001E89E0[];
extern u8 D_001E89C8[];
extern void DebugPrint(char *, ...);
extern void FlushCache(s32);
extern struct DmaTag *sceDmaGetChan(s32);
extern void sceDmaSend(struct DmaTag *, s32);

void vu1_send_chain(void) __asm__("FUN_002336a0");

void vu1_send_chain(void) {
    struct DmaTag *chan;
    s32 chain_size;
    s32 remaining;
    s32 overflow;

    chain_size = D_00160EF8[D_00160F10];
    overflow = 0;
    remaining = (s32)D_00160F00.p - chain_size;
    D_00160EE0 |= 0x1F;
    if (D_00160F14 < remaining) {
        D_00160F14 = remaining;
        if (D_00160F0C < remaining) {
            DebugPrint(D_001E89C8);
            overflow = 1;
        }
    }
    if (overflow == 0) {
        D_00160F00.p->unk0 = 0x70000000;
        D_00160F00.p->unk4 = 0;
        D_00160F00.p->unk8 = 0;
        D_00160F00.p->unkC = 0;
        chan = sceDmaGetChan(1);
        chan->unk0 |= 0xC0;
        FlushCache(0);
        sceDmaSend(chan, D_00160EF8[D_00160F10]);
        return;
    }
    D_00160EE0 = 0;
}

extern __typeof__(vu1_send_chain) func_002336A0 __attribute__((alias("FUN_002336a0")));

extern void SpinWait(s32);
extern void DebugPrint(char *, ...);
extern void FUN_001f21c0(void);
void vu1_sync_chain(s32 mask) __asm__("FUN_002337b0");

void vu1_sync_chain(s32 mask) {
    s32 i;

    for (i = 0; D_00160EE0 & mask; i++) {
        SpinWait(0x400);
        if (i > 100000) {
            DebugPrint(D_001E89E0);
            FUN_001f21c0();
            break;
        }
    }
}

extern __typeof__(vu1_sync_chain) func_002337B0 __attribute__((alias("FUN_002337b0")));
