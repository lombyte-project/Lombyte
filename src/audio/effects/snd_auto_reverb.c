#include "types.h"
extern s32 snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a,
                                        s32 b) __asm__("func_0012E6E0");
void snd_auto_reverb(s32 arg0, s32 arg1, s32 arg2, s32 arg3) __asm__("FUN_0012efe0");

void snd_auto_reverb(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 sp_slot[0x10];
    *(s32 *)(sp_slot + 0) = arg0;
    *(s32 *)(sp_slot + 4) = arg1;
    *(s32 *)(sp_slot + 8) = arg2;
    *(s32 *)(sp_slot + 12) = arg3;
    snd_send_iop_command_no_wait(16, 16, sp_slot, 0, 0);
}
