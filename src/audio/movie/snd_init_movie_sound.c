#include "types.h"
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void snd_init_movie_sound(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                          s32 arg5) __asm__("FUN_0012f068");

void snd_init_movie_sound(s32 transfer_block_size, s32 staging_buffer_size, s32 staging_block_size, s32 initial_offset, s32 buffer_count, s32 stream_count) {
    s32 sp_slot[6];

    sp_slot[0] = transfer_block_size;
    sp_slot[1] = staging_buffer_size;
    sp_slot[2] = staging_block_size;
    sp_slot[3] = initial_offset;
    sp_slot[4] = buffer_count;
    sp_slot[5] = stream_count;
    snd_send_iop_command_and_wait(0x3B, 0x18, sp_slot);
}
