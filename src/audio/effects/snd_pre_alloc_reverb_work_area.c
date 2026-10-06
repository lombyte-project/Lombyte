#include "types.h"
extern s32 snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a,
                                        s32 b) __asm__("func_0012E6E0");
void snd_pre_alloc_reverb_work_area(s32 arg0, s32 arg1) __asm__("FUN_0012efa8");

void snd_pre_alloc_reverb_work_area(s32 arg0, s32 arg1) {
    u8 sp_slot[0x10];
    *(s32 *)(sp_slot + 0) = arg0;
    *(s32 *)(sp_slot + 4) = arg1;
    snd_send_iop_command_no_wait(81, 8, sp_slot, 0, 0);
}
