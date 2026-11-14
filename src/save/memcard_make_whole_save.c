#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/save/mc_data.h"
#include "rnc/save/memcard_get_data_size.h"
#include "rnc/save/memory_card_state.h"

extern s32 memcard_prepare_data(u8 *, s32, u8 *) __asm__("func_0020AD78");

void memcard_make_whole_save(u8 *cursor) __asm__("FUN_0020abb0");

void memcard_make_whole_save(u8 *cursor) {
    s32 index;
    s32 level_record_bytes;

    ((struct MemoryCardSaveSizes *)cursor)->main_size = memcard_get_data_size(mc_gamedata);
    ((struct MemoryCardSaveSizes *)cursor)->record_size = memcard_get_data_size(mc_leveldata);
    cursor += sizeof(struct MemoryCardSaveSizes);
    cursor += memcard_prepare_data(cursor, 0, mc_gamedata);
    index = 0;
    do {
        level_record_bytes = memcard_prepare_data(cursor, index, mc_leveldata);
        cursor += level_record_bytes;
        index++;
    } while (index < 0x14);
}

extern __typeof__(memcard_make_whole_save) func_0020ABB0 __attribute__((alias("FUN_0020abb0")));
