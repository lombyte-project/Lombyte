#include "types.h"
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void snd_update_movie_adpcm(s32 arg0, s32 arg1) __asm__("FUN_0012f148");

void snd_update_movie_adpcm(s32 byte_count, s32 staging_byte_offset) {
    s32 local[2];

    local[0] = byte_count;
    local[1] = staging_byte_offset;
    snd_send_iop_command_and_wait(0x5A, 8, local);
}
