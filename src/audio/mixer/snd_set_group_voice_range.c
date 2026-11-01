#include "types.h"

extern s32
snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a, s32 b) __asm__("func_0012E6E0");
void snd_set_group_voice_range(s32 payload_word_0,
                               s32 payload_word_1,
                               s32 payload_word_2) __asm__("FUN_0012e2b8");

void snd_set_group_voice_range(s32 payload_word_0, s32 payload_word_1, s32 payload_word_2) {
    s32 command[4];
    command[0] = payload_word_0;
    command[1] = payload_word_1;
    command[2] = payload_word_2;
    snd_send_iop_command_no_wait(78, 12, command, 0, 0);
}
