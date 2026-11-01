#include "types.h"

extern s32
snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a, s32 b) __asm__("func_0012E6E0");
void snd_set_playback_mode(s32 playback_mode) __asm__("FUN_0012e240");

void snd_set_playback_mode(s32 playback_mode) {
    s32 command[4];
    command[0] = playback_mode;
    snd_send_iop_command_no_wait(11, 4, command, 0, 0);
}
