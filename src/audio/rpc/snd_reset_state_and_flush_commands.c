#include "types.h"
extern s32 D_0015ECC4 __attribute__((sda));
extern s32 snd_flush_sound_commands() __asm__("FUN_0012dc80");
void snd_reset_state_and_flush_commands(s32 arg0) __asm__("FUN_0012eb00");

void snd_reset_state_and_flush_commands(s32 arg0) {
    D_0015ECC4 = 0;
    snd_flush_sound_commands();
}

extern void func_0012EB00(s32 arg0) __attribute__((alias("FUN_0012eb00")));

/* Recovered original symbol name. */
extern __typeof__(snd_reset_state_and_flush_commands) snd_UnkFunction_0012eb00
    __attribute__((alias("FUN_0012eb00")));
