#include "types.h"
extern s32 snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a,
                                        s32 b) __asm__("func_0012E6E0");
void snd_sound_is_still_playing_cb(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_0012e448");

void snd_sound_is_still_playing_cb(s32 arg0, s32 arg1, s32 arg2) {
    u8 sp_slot[0x10];
    *(s32 *)sp_slot = arg0;
    snd_send_iop_command_no_wait(25, 4, sp_slot, arg1, arg2);
}
