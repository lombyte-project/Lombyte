#include "types.h"
#include "sda.h"

#include "rnc/storage/memory_card/memory_card_state.h"
extern u8 D_0015EE98[] MACRO_ADDR;
extern s32 load_and_initialize_level_chunk() __asm__("func_00209370");
extern s32 memcard_make_whole_save() __asm__("func_0020ABB0");
extern s32 sceCdReadClock();
extern s32 sceScfGetLocalTimefromRTC();
void FUN_00226a70(s32 save_data, s32 slot) {
    load_and_initialize_level_chunk();
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    memcard_make_whole_save(save_data);
    memory_card_state.active_card = 0;
    memory_card_state.card[0].save_index = slot;
    *(s32 *)((u8 *)&memory_card_state + slot * 0x1C + 0x20) = 0;
    memory_card_state.unkF4 = 1;
    memory_card_state.buf = save_data;
    if (memory_card_state.pending_state < 0) {
        memory_card_state.pending_card = 0;
        memory_card_state.pending_state = 0x13;
    }
}
