#include "types.h"
extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");
void snd_play_sound_vol_pan_pmpb(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                                 s32 a7) __asm__("FUN_0012e308");

void snd_play_sound_vol_pan_pmpb(s32 request_word_0, s32 request_word_1, s32 request_word_2, s32 request_word_3, s32 request_word_4, s32 request_word_5, s32 callback_address, s32 callback_context_address) {
    s32 sound_request_payload[6];

    sound_request_payload[0] = request_word_0;
    sound_request_payload[1] = request_word_1;
    sound_request_payload[2] = request_word_2;
    sound_request_payload[3] = request_word_3;
    sound_request_payload[4] = request_word_4;
    sound_request_payload[5] = request_word_5;
    snd_send_iop_command_no_wait(0x11, 0x18, sound_request_payload, callback_address, callback_context_address);
}

extern __typeof__(snd_play_sound_vol_pan_pmpb) func_0012E308 __attribute__((alias("FUN_0012e308")));
