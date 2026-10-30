#include "types.h"
#include "rnc/globals.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/audio/voice_pool.h"

extern s32   D_001940D4[];
extern void  count_vsync() __asm__("FUN_0012f1c8");
extern void  play_mpeg_movie(s32, s32, s32, s32, s32) __asm__("func_0023A3B8");
extern s32   sceGsSyncV(s32);
extern void  sceGsSyncPath(s32, s32) __asm__("FUN_00120558");
extern void *sceGsSyncVCallback(void *);
extern void  fade_to_black(s32) __asm__("func_001F4A58");
void         play_level_transition_movie(s32 movie_index) __asm__("FUN_00231608");

void play_level_transition_movie(s32 movie_index) {
    s32 stream_size;
    s32 stream_start;
    s32 archive_base;

    if (pal_mode != 0) {
        stream_start = disc_table.movies_alt[movie_index].sector;
        stream_size = disc_table.movies_alt[movie_index].size;
    } else {
        stream_start = disc_table.movies[movie_index].sector;
        stream_size = disc_table.movies[movie_index].size;
    }
    archive_base = D_001940D4[0];
    voice_pool.reverb_commands |= 8;
    play_mpeg_movie(stream_start,
                    stream_size,
                    (archive_base + 0x3F) & ~0x3F,
                    (archive_base + 0x30003F) & ~0x3F,
                    game_language);
    sceGsSyncV(0);
    sceGsSyncPath(0, 0);
    sceGsSyncVCallback(count_vsync);
    fade_to_black(4);
    voice_pool.reverb_commands |= 0x10;
}

extern __typeof__(play_level_transition_movie) func_00231608 __attribute__((alias("FUN_00231608")));
