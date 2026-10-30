#include "types.h"

extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");
void       snd_stop_sound(s32 sound_handle) __asm__("FUN_0012e368");

void snd_stop_sound(s32 sound_handle) {
    s32 sound_handle_payload[4];

    sound_handle_payload[0] = sound_handle;
    snd_send_iop_command_no_wait(0x15, 4, sound_handle_payload, 0, 0);
}

extern __typeof__(snd_stop_sound) func_0012E368 __attribute__((alias("FUN_0012e368")));
