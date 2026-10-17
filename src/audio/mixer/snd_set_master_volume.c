#include "types.h"
extern s32 snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a,
                                        s32 b) __asm__("func_0012E6E0");
void snd_set_master_volume(s32 arg0, s32 arg1) __asm__("FUN_0012e208");

void snd_set_master_volume(s32 group, s32 volume) {
    u8 sp_slot[0x10];
    *(s32 *)sp_slot = group;
    *(s32 *)(sp_slot + 4) = volume;
    snd_send_iop_command_no_wait(9, 8, sp_slot, 0, 0);
}
