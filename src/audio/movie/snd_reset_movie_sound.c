#include "types.h"

extern s32 snd_send_iop_command_and_wait() __asm__("FUN_0012e548");
void       snd_reset_movie_sound(void) __asm__("FUN_0012f0a8");

void snd_reset_movie_sound(void) {
    snd_send_iop_command_and_wait(0x3D, 0, 0);
}

extern void func_0012F0A8(void) __attribute__((alias("FUN_0012f0a8")));
