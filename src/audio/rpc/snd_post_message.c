#include "types.h"
extern s32 D_0015ECA0;
extern s32 D_0015ECC0;
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
void snd_post_message(void) __asm__("FUN_0012e9a0");

void snd_post_message(void) {
    s32 *temp_4_11;
    s32 idx;
    u8 *base;

    idx = D_0015ECC0;
    base = (u8 *)&D_0015ECA0;
    temp_4_11 = *(s32 **)(base + (idx * 4));
    *temp_4_11 += 1;
    snd_flush_sound_commands(temp_4_11);
}

extern __typeof__(snd_post_message) func_0012E9A0 __attribute__((alias("FUN_0012e9a0")));

/* Recovered original symbol name. */
extern __typeof__(snd_post_message) snd_PostMessage __attribute__((alias("FUN_0012e9a0")));
