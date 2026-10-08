#include "types.h"
#include "rnc/rendering/dma_tag.h"

/* The SDK DMA channel registers; only CHCR (offset 0) is touched here. */
struct DmaChannel {
    s32 chcr;
    s32 pad_04[3];
};

/* The DMA-busy mask this file owns: both functions below read it
   gp-relative, which needs its definition ahead of them. */
volatile s32 D_00160EE0 = 0;
extern s32 D_00160EF8[];
extern s32 D_00160F0C;
extern s32 D_00160F10;
extern s32 D_00160F14;
extern char D_001E89E0[];
extern u8 D_001E89C8[];
extern void DebugPrint(char *, ...);
extern void FlushCache(s32);
extern struct DmaChannel *sceDmaGetChan(s32);
extern void sceDmaSend(struct DmaChannel *, s32);

void vu1_send_chain(void) __asm__("FUN_002336a0");

void vu1_send_chain(void) {
    struct DmaChannel *channel;
    s32 chain_size;
    s32 remaining;
    s32 overflow;

    chain_size = D_00160EF8[D_00160F10];
    overflow = 0;
    remaining = (s32)render_packet_cursor.tag - chain_size;
    D_00160EE0 |= 0x1F;
    if (D_00160F14 < remaining) {
        D_00160F14 = remaining;
        if (D_00160F0C < remaining) {
            DebugPrint(D_001E89C8);
            overflow = 1;
        }
    }
    if (overflow == 0) {
        render_packet_cursor.tag->tag = 0x70000000;
        render_packet_cursor.tag->addr = 0;
        render_packet_cursor.tag->vif0 = 0;
        render_packet_cursor.tag->vif1 = 0;
        channel = sceDmaGetChan(1);
        channel->chcr |= 0xC0;
        FlushCache(0);
        sceDmaSend(channel, D_00160EF8[D_00160F10]);
        return;
    }
    D_00160EE0 = 0;
}

extern __typeof__(vu1_send_chain) func_002336A0 __attribute__((alias("FUN_002336a0")));

extern void SpinWait(s32);
extern void DebugPrint(char *, ...);
extern void reset_graphics(void) __asm__("FUN_001f21c0");
void vu1_sync_chain(s32 mask) __asm__("FUN_002337b0");

void vu1_sync_chain(s32 mask) {
    s32 poll_count;

    for (poll_count = 0; D_00160EE0 & mask; poll_count++) {
        SpinWait(0x400);
        if (poll_count > 100000) {
            DebugPrint(D_001E89E0);
            reset_graphics();
            break;
        }
    }
}

extern __typeof__(vu1_sync_chain) func_002337B0 __attribute__((alias("FUN_002337b0")));
