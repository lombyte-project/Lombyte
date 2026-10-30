#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/save/mc_data.h"
#include "rnc/save/memcard_get_data_size.h"

extern char out_of_date_save_message[] __asm__("D_001E81D0");
extern void DebugPrint(char *, ...);
extern void memcard_restore_data(u8 *, s32, void *) __asm__("func_0020AF20");
void        memcard_restore_game(u8 *p) __asm__("FUN_00209298");

void memcard_restore_game(u8 *save_buffer) {
    s32 size0 = memcard_get_data_size(mc_gamedata);
    s32 size1 = memcard_get_data_size(mc_leveldata);
    s32 saved_game_span_bytes = ((s32 *)save_buffer)[0];
    s32 i;

    if (saved_game_span_bytes != size0 || ((s32 *)save_buffer)[1] != size1) {
        DebugPrint(out_of_date_save_message);
        return;
    }
    save_buffer += 8;
    memcard_restore_data(save_buffer, 0, mc_gamedata);
    save_buffer += saved_game_span_bytes;
    for (i = 0; i < 20; i++) {
        memcard_restore_data(save_buffer, i, mc_leveldata);
        save_buffer += size1;
    }
}

extern __typeof__(memcard_restore_game) func_00209298 __attribute__((alias("FUN_00209298")));
