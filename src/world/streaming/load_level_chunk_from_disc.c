#include "types.h"
#include "rnc/storage/disc_table.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
extern u8 D_1FF7FF0[];
extern void submit_audio_stream_io_request(void *, s32, s32) __asm__("func_00216728");
s32 load_level_chunk_from_disc(void) __asm__("FUN_002043b0");

s32 load_level_chunk_from_disc(void) {
    level_root =
        (s32 *)((s32)((u32)D_1FF7FF0 - (((disc_table.level_chunk.size << 11) + 0x1057) & 0xFFFFF000)) &
                -0x10);
    *level_root = 0x60;
    submit_audio_stream_io_request((u8 *)level_root + *level_root, disc_table.level_chunk.sector,
                                   disc_table.level_chunk.size);
    return 1;
}

extern __typeof__(load_level_chunk_from_disc) func_002043B0 __attribute__((alias("FUN_002043b0")));
