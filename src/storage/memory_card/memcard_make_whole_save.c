#include "types.h"

extern u8 mc_gamedata[] __asm__("D_001A04C0");
extern u8 mc_leveldata[] __asm__("D_001A07C0");
extern s32 get_dma_packet_span_bytes(u8 *) __asm__("GetDmaPacketSpanBytes");
extern s32 memcard_prepare_data(u8 *, s32, u8 *) __asm__("func_0020AD78");

void memcard_make_whole_save(u8 *cursor) __asm__("FUN_0020abb0");

void memcard_make_whole_save(u8 *cursor) {
    s32 index;
    s32 size;

    *(s32 *)(cursor + 0x0) = get_dma_packet_span_bytes(mc_gamedata);
    *(s32 *)(cursor + 0x4) = get_dma_packet_span_bytes(mc_leveldata);
    cursor += 8;
    cursor += memcard_prepare_data(cursor, 0, mc_gamedata);
    index = 0;
    do {
        size = memcard_prepare_data(cursor, index, mc_leveldata);
        cursor += size;
        index++;
    } while (index < 0x14);
}

extern __typeof__(memcard_make_whole_save) func_0020ABB0 __attribute__((alias("FUN_0020abb0")));
