#include "types.h"
extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");
void snd_set_reverb_ex(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) __asm__("FUN_0012ef68");

void snd_set_reverb_ex(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    s32 sp_slot[5];
    sp_slot[0] = a0;
    sp_slot[1] = a1;
    sp_slot[2] = a2;
    sp_slot[3] = a3;
    sp_slot[4] = a4;
    snd_send_iop_command_no_wait(0x50, 0x14, sp_slot, 0, 0);
}

extern __typeof__(snd_set_reverb_ex) func_0012EF68 __attribute__((alias("FUN_0012ef68")));

/* Recovered original symbol name. */
extern __typeof__(snd_set_reverb_ex) snd_SetReverbEx __attribute__((alias("FUN_0012ef68")));
