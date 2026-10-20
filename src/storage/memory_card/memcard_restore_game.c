#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/storage/memory_card/mc_data.h"
extern char D_001E81D0[];
extern s32 get_dma_packet_span_bytes(void *) __asm__("GetDmaPacketSpanBytes");
extern void DebugPrint(char *, ...);
extern void memcard_restore_data(u8 *, s32, void *) __asm__("func_0020AF20");
void memcard_restore_game(u8 *p) __asm__("FUN_00209298");

void memcard_restore_game(u8 *save_buffer) {
    s32 size0 = get_dma_packet_span_bytes(mc_gamedata);
    s32 size1 = get_dma_packet_span_bytes(mc_leveldata);
    s32 len0 = ((s32 *)save_buffer)[0];
    s32 i;

    if (len0 != size0 || ((s32 *)save_buffer)[1] != size1) {
        DebugPrint(D_001E81D0);
        return;
    }
    save_buffer += 8;
    memcard_restore_data(save_buffer, 0, mc_gamedata);
    save_buffer += len0;
    for (i = 0; i < 20; i++) {
        memcard_restore_data(save_buffer, i, mc_leveldata);
        save_buffer += size1;
    }
}

extern __typeof__(memcard_restore_game) func_00209298
    __attribute__((alias("FUN_00209298")));
