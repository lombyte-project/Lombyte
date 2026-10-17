#include "types.h"
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void snd_start_movie_sound(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) __asm__("FUN_0012f108");

void snd_start_movie_sound(s32 driver_handle, s32 transfer_byte_count, s32 start_byte_offset, s32 sample_rate, s32 channel_count) {
    s32 buf[5];

    buf[0] = driver_handle;
    buf[1] = transfer_byte_count;
    buf[2] = start_byte_offset;
    buf[3] = sample_rate;
    buf[4] = channel_count;
    snd_send_iop_command_and_wait(0x3E, 0x14, buf);
}

extern __typeof__(snd_start_movie_sound) func_0012F108 __attribute__((alias("FUN_0012f108")));
