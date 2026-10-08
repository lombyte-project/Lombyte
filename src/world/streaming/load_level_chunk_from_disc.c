#include "types.h"
#include "rnc/storage/disc_table.h"
extern u8 D_1FF7FF0[];
extern s32 *D_0015EE4C;
extern void submit_audio_stream_io_request(void *, s32, s32) __asm__("func_00216728");
s32 load_level_chunk_from_disc(void) __asm__("FUN_002043b0");

s32 load_level_chunk_from_disc(void) {
    D_0015EE4C =
        (s32 *)((s32)((u32)D_1FF7FF0 - (((disc_table.level_chunk.size << 11) + 0x1057) & 0xFFFFF000)) &
                -0x10);
    *D_0015EE4C = 0x60;
    submit_audio_stream_io_request((u8 *)D_0015EE4C + *D_0015EE4C, disc_table.level_chunk.sector,
                                   disc_table.level_chunk.size);
    return 1;
}

extern __typeof__(load_level_chunk_from_disc) func_002043B0 __attribute__((alias("FUN_002043b0")));
