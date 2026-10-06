#include "types.h"
extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");
void snd_stop_sound(s32 arg0) __asm__("FUN_0012e368");

void snd_stop_sound(s32 arg0) {
    s32 buf[4];

    buf[0] = arg0;
    snd_send_iop_command_no_wait(0x15, 4, buf, 0, 0);
}

extern __typeof__(snd_stop_sound) func_0012E368 __attribute__((alias("FUN_0012e368")));
