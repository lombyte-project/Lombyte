/* Ported from rac1-decomp (src/game/bmain.c, func_001E9808). */
#include "sda.h"
extern int D_0015ED80 MACRO_ADDR;
extern int D_0015EED8 MACRO_ADDR;
extern int D_0015F604 MACRO_ADDR;
extern int D_0015EE88 MACRO_ADDR;
typedef struct {
    int b;
    int a;
} LevelLoad;
typedef struct {
    char pad0[0x1A78];
    LevelLoad normal[4];
    LevelLoad alt[4];
} Globals137C80;
extern Globals137C80 D_00137B80;
extern char D_0013E550[];
extern char D_0013D290[];
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
extern void FUN_00120558(int, int);
extern int count_vsync(void) __asm__("FUN_0012f1c8");
extern void sceGsSyncVCallback(void (*)(void));
extern void hud_send_texture(int, int, int, int, int, int) __asm__("func_00200B10");
/* Start level `level`: pick its two load parameters from the level
   table in D_00137B80 (the alternate set while D_0015ED80 is set), stop
   sound and wait for the loader (func_002093D8) to go idle, then load it
   (func_0023A3B8) and reset the display state. */
void start_level(int level) __asm__("FUN_001e9488");

void start_level(int level) {
    int a;
    int b;
    int t;

    if (level < 0) {
        return;
    }
    if (D_0015ED80 != 0) {
        b = D_00137B80.alt[level].b;
        a = D_00137B80.alt[level].a;
    } else {
        b = D_00137B80.normal[level].b;
        a = D_00137B80.normal[level].a;
    }
    D_0015EED8 = 2;
    D_0013E550[0x6B] |= 8;
    FlushCache(0);
    sound_stop_all_sounds();
    music_stop();
    fade_to_black(scale_game_frames(12));
    D_0015F604 = 1;
    FlushCache(0);
    sound_stop_all_sounds();
    music_stop();
    snd_stream_safe_cd_sync(0);
    for (;;) {
        char *ld = D_0013D290;
        if (*(int *)(ld + 0xD4) < 3 && *(int *)(ld + 0xDC) < 0) {
            break;
        }
        memcard_update_state();
    }
    {
        char *g = D_001940C0;
        t = *(int *)(g + 0x1C);
    }
    play_mpeg_movie(b, a, t + 0x100000, t + 0x400000, 0);
    sceCdSync(0);
    sceGsSyncV(0);
    FUN_00120558(0, 0);
    sceGsSyncVCallback(count_vsync);
    hud_send_texture(0x1000000, D_0015EE88, 0x1B, 6, 6, 1);
    D_0015EED8 = 0;
    fade_to_black(4);
    D_0015F604 = 0;
    D_0013E550[0x6B] |= 0x10;
}

extern __typeof__(start_level) func_001E9488 __attribute__((alias("FUN_001e9488")));
