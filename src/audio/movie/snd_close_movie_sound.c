#include "types.h"
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void snd_close_movie_sound(void) __asm__("FUN_0012f0e0");

void snd_close_movie_sound(void) {
    snd_send_iop_command_and_wait(0x3C, 0, 0);
}

extern void func_0012F0E0(void) __attribute__((alias("FUN_0012f0e0")));
