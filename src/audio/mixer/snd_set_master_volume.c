#include "types.h"

extern s32
snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a, s32 b) __asm__("func_0012E6E0");
void snd_set_master_volume(s32 group, s32 volume) __asm__("FUN_0012e208");

void snd_set_master_volume(s32 group, s32 volume) {
    s32 command[4];
    command[0] = group;
    command[1] = volume;
    snd_send_iop_command_no_wait(9, 8, command, 0, 0);
}
