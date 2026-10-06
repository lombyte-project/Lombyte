#include "types.h"
extern s32 snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a,
                                        s32 b) __asm__("func_0012E6E0");
void snd_unload_bank(s32 arg0) __asm__("FUN_0012e1d8");

void snd_unload_bank(s32 arg0) {
    u8 sp_slot[0x10];
    *(s32 *)sp_slot = arg0;
    snd_send_iop_command_no_wait(6, 4, sp_slot, 0, 0);
}

/* Recovered original symbol name. */
extern __typeof__(snd_unload_bank) snd_UnloadBank __attribute__((alias("FUN_0012e1d8")));
