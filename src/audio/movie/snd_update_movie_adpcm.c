#include "types.h"
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void snd_update_movie_adpcm(s32 arg0, s32 arg1) __asm__("FUN_0012f148");

void snd_update_movie_adpcm(s32 arg0, s32 arg1) {
    s32 local[2];

    local[0] = arg0;
    local[1] = arg1;
    snd_send_iop_command_and_wait(0x5A, 8, local);
}
