#include "types.h"

extern u8 D_001A04C0[];
extern u8 D_001A07C0[];
extern s32 GetDmaPacketSpanBytes(u8 *);
extern s32 memcard_prepare_data(u8 *, s32, u8 *) __asm__("func_0020AD78");

void memcard_make_whole_save(u8 *arg0) __asm__("FUN_0020abb0");

void memcard_make_whole_save(u8 *arg0) {
    s32 index;
    s32 size;

    *(s32 *)(arg0 + 0x0) = GetDmaPacketSpanBytes(D_001A04C0);
    *(s32 *)(arg0 + 0x4) = GetDmaPacketSpanBytes(D_001A07C0);
    arg0 += 8;
    arg0 += memcard_prepare_data(arg0, 0, D_001A04C0);
    index = 0;
    do {
        size = memcard_prepare_data(arg0, index, D_001A07C0);
        arg0 += size;
        index++;
    } while (index < 0x14);
}

extern __typeof__(memcard_make_whole_save) func_0020ABB0 __attribute__((alias("FUN_0020abb0")));
