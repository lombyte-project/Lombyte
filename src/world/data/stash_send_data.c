#include "types.h"

struct SifDmaData { u32 data; u32 addr; u32 size; u32 mode; };
struct StashEntry { s32 addr; s32 count; s32 tag; s32 pad; };
struct Stash { s32 base; s32 size; u8 pad8[0x28]; s32 cur; s32 n; };

extern struct Stash D_001DD1A0;
extern struct StashEntry D_001DD1D8[];
extern u32 sceSifSetDma(struct SifDmaData *, s32);

s32 stash_send_data(u32 src, s32 qw, s32 count, s32 tag) __asm__("FUN_00232e40");

s32 stash_send_data(u32 src, s32 qw, s32 count, s32 tag) {
    struct SifDmaData dma;
    s32 bytes;
    s32 n;
    s32 i;
    s32 cur;
    s32 next;

    bytes = count << 4;
    if (D_001DD1A0.size - (D_001DD1A0.cur - D_001DD1A0.base) < bytes) {
        return -1;
    }
    if (D_001DD1A0.n == 64) {
        return -2;
    }
    dma.data = src;
    dma.addr = D_001DD1A0.cur;
    dma.size = qw << 4;
    dma.mode = 0;
    sceSifSetDma(&dma, 1);
    n = D_001DD1A0.n;
    i = n;
    cur = D_001DD1A0.cur;
    next = cur + bytes;
    n++;
    D_001DD1A0.n = n;
    D_001DD1D8[i].addr = cur;
    D_001DD1D8[i].count = count;
    D_001DD1D8[i].tag = tag;
    D_001DD1A0.cur = next;

    return i;
}

extern __typeof__(stash_send_data) func_00232E40 __attribute__((alias("FUN_00232e40")));
