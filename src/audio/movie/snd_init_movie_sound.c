#include "types.h"
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void snd_init_movie_sound(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                          s32 arg5) __asm__("FUN_0012f068");

void snd_init_movie_sound(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp_slot[6];

    sp_slot[0] = arg0;
    sp_slot[1] = arg1;
    sp_slot[2] = arg2;
    sp_slot[3] = arg3;
    sp_slot[4] = arg4;
    sp_slot[5] = arg5;
    snd_send_iop_command_and_wait(0x3B, 0x18, sp_slot);
}
