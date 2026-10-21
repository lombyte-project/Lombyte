/* Ported from rac1-decomp (src/game/bmain.c, func_001E9808). */
#include "sda.h"
#include "rnc/storage/disc_table.h"
#include "types.h"
extern int decode_mode_macro __asm__("D_0015EED8") MACRO_ADDR;
extern char D_0013E550[];
#include "rnc/storage/memory_card/memory_card_state.h"
extern char D_001940C0[];
extern void FlushCache(int);
extern void sound_stop_all_sounds(void) __asm__("func_0022DCD0");
extern void music_stop(void) __asm__("func_00215EE8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void fade_to_black(int) __asm__("func_001F4A58");
extern void snd_stream_safe_cd_sync(int) __asm__("func_0012EE08");
extern void memcard_update_state(void) __asm__("func_002093D8");
extern void play_mpeg_movie(int, int, int, int, int) __asm__("func_0023A3B8");
extern void sceCdSync(int);
extern void sceGsSyncV(int);
extern void wait_for_graphics_pipeline_idle(int, int) __asm__("FUN_00120558");
extern int count_vsync(void) __asm__("FUN_0012f1c8");
extern void sceGsSyncVCallback(void (*)(void));
extern void hud_send_texture(int, int, int, int, int, int) __asm__("func_00200B10");
/* Start level `level`: pick its two load parameters from the level
   table in disc_table (the alternate set while D_0015ED80 is set), stop
   sound and wait for the loader (memcard_update_state) to go idle, then load it
   (play_mpeg_movie) and reset the display state. */
void start_level(int level) __asm__("FUN_001e9488");

extern s32 pal_mode __asm__("D_0015ED80") MACRO_ADDR;
extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;
extern s32 depth_buffer_address __asm__("D_0015EE88") MACRO_ADDR;
void start_level(int level) {
    int movie_size_bytes;
    int movie_start_sector;
    int movie_work_area_base;

    if (level < 0) {
        return;
    }
    if (pal_mode != 0) {
        movie_start_sector = disc_table.start_movies_alt[level].sector;
        movie_size_bytes = disc_table.start_movies_alt[level].size;
    } else {
        movie_start_sector = disc_table.start_movies[level].sector;
        movie_size_bytes = disc_table.start_movies[level].size;
    }
    decode_mode_macro = 2;
    D_0013E550[0x6B] |= 8;
    FlushCache(0);
    sound_stop_all_sounds();
    music_stop();
    fade_to_black(scale_game_frames(12));
    game_mode_macro = 1;
    FlushCache(0);
    sound_stop_all_sounds();
    music_stop();
    snd_stream_safe_cd_sync(0);
    for (;;) {
        struct MemoryCardState *ld = &memory_card_state;
        if (ld->state < 3 && ld->pending_state < 0) {
            break;
        }
        memcard_update_state();
    }
    {
        char *movie_work_area_record = D_001940C0;
        movie_work_area_base = *(int *)(movie_work_area_record + 0x1C);
    }
    play_mpeg_movie(movie_start_sector, movie_size_bytes, movie_work_area_base + 0x100000, movie_work_area_base + 0x400000, 0);
    sceCdSync(0);
    sceGsSyncV(0);
    wait_for_graphics_pipeline_idle(0, 0);
    sceGsSyncVCallback(count_vsync);
    hud_send_texture(0x1000000, depth_buffer_address, 0x1B, 6, 6, 1);
    decode_mode_macro = 0;
    fade_to_black(4);
    game_mode_macro = 0;
    D_0013E550[0x6B] |= 0x10;
}

extern __typeof__(start_level) func_001E9488 __attribute__((alias("FUN_001e9488")));
