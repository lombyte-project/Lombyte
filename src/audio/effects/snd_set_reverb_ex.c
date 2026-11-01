#include "types.h"

extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");
void       snd_set_reverb_ex(s32 reverb_request_word_0,
                             s32 reverb_request_word_1,
                             s32 reverb_request_word_2,
                             s32 reverb_request_word_3,
                             s32 reverb_request_word_4) __asm__("FUN_0012ef68");

void snd_set_reverb_ex(s32 reverb_request_word_0,
                       s32 reverb_request_word_1,
                       s32 reverb_request_word_2,
                       s32 reverb_request_word_3,

                       s32 reverb_request_word_4) {
    s32 command[5];
    command[0] = reverb_request_word_0;
    command[1] = reverb_request_word_1;
    command[2] = reverb_request_word_2;
    command[3] = reverb_request_word_3;
    command[4] = reverb_request_word_4;
    snd_send_iop_command_no_wait(0x50, 0x14, command, 0, 0);
}

extern __typeof__(snd_set_reverb_ex) func_0012EF68 __attribute__((alias("FUN_0012ef68")));

/* Recovered original symbol name. */
extern __typeof__(snd_set_reverb_ex) snd_SetReverbEx __attribute__((alias("FUN_0012ef68")));
