#include "types.h"

extern s32
snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a, s32 b) __asm__("func_0012E6E0");
void snd_auto_reverb(s32 reverb_payload_word_0,
                     s32 reverb_payload_word_1,
                     s32 reverb_payload_word_2,
                     s32 reverb_payload_word_3) __asm__("FUN_0012efe0");

void snd_auto_reverb(s32 reverb_payload_word_0,
                     s32 reverb_payload_word_1,
                     s32 reverb_payload_word_2,

                     s32 reverb_payload_word_3) {
    s32 command[4];
    command[0] = reverb_payload_word_0;
    command[1] = reverb_payload_word_1;
    command[2] = reverb_payload_word_2;
    command[3] = reverb_payload_word_3;
    snd_send_iop_command_no_wait(16, 16, command, 0, 0);
}
