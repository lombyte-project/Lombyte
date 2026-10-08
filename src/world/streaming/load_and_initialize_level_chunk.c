#include "types.h"
#include "rnc/globals.h"
#include "rnc/storage/disc_table.h"
struct Chunk {
    u8 pad0[0x10];
    s32 offset;
};
extern void calculate_ring_buffer_bounds(s32, struct Chunk **, s32 *) __asm__("func_001FD6E0");
extern void update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");
extern s32 load(struct Chunk *, s32, s32) __asm__("func_00216828");
extern void memcard_restore_game(void *) __asm__("func_00209298");
void load_and_initialize_level_chunk(void) __asm__("FUN_00209370");

void load_and_initialize_level_chunk(void) {
    struct Chunk *chunk;
    s32 size;

    calculate_ring_buffer_bounds(disc_table.memcard_data.size << 11, &chunk, &size);
    update_audio_stream_until_idle(1);
    load(chunk, disc_table.memcard_data.sector, disc_table.memcard_data.size);
    memcard_restore_game((u8 *)chunk + chunk->offset);
    current_level_index = 0;
}

extern __typeof__(load_and_initialize_level_chunk) func_00209370
    __attribute__((alias("FUN_00209370")));
