#include "types.h"
struct Chunk {
    u8 pad0[0x10];
    s32 offset;
};
struct Loader {
    u8 pad0[0x10];
    s32 dest;
    s32 sector;
};
extern struct Loader D_00137B80;
extern s32 D_0015ED84;
extern void calculate_ring_buffer_bounds(s32, struct Chunk **, s32 *) __asm__("func_001FD6E0");
extern void update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");
extern s32 load(struct Chunk *, s32, s32) __asm__("func_00216828");
extern void memcard_restore_game(void *) __asm__("func_00209298");
void load_and_initialize_level_chunk(void) __asm__("FUN_00209370");

void load_and_initialize_level_chunk(void) {
    struct Chunk *chunk;
    s32 size;

    calculate_ring_buffer_bounds(D_00137B80.sector << 11, &chunk, &size);
    update_audio_stream_until_idle(1);
    load(chunk, D_00137B80.dest, D_00137B80.sector);
    memcard_restore_game((u8 *)chunk + chunk->offset);
    D_0015ED84 = 0;
}

extern __typeof__(load_and_initialize_level_chunk) func_00209370
    __attribute__((alias("FUN_00209370")));
