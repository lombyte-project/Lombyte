#include "types.h"

extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void       snd_init_movie_sound(s32 transfer_block_size,
                                s32 staging_buffer_size,
                                s32 staging_block_size,
                                s32 initial_offset,
                                s32 buffer_count,
                                s32 stream_count) __asm__("FUN_0012f068");

void snd_init_movie_sound(s32 transfer_block_size,
                          s32 staging_buffer_size,
                          s32 staging_block_size,
                          s32 initial_offset,
                          s32 buffer_count,

                          s32 stream_count) {
    s32 command[6];

    command[0] = transfer_block_size;
    command[1] = staging_buffer_size;
    command[2] = staging_block_size;
    command[3] = initial_offset;
    command[4] = buffer_count;
    command[5] = stream_count;
    snd_send_iop_command_and_wait(0x3B, 0x18, command);
}
