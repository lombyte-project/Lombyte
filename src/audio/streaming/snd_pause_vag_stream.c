#include "types.h"
extern s32 snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a,
                                        s32 b) __asm__("func_0012E6E0");
void snd_pause_vag_stream(s32 arg0) __asm__("FUN_0012ec70");

void snd_pause_vag_stream(s32 arg0) {
    u8 sp_slot[0x10];
    *(s32 *)(sp_slot + 0) = arg0;
    snd_send_iop_command_no_wait(45, 4, sp_slot, 0, 0);
}

/* Recovered original symbol name. */
extern __typeof__(snd_pause_vag_stream) snd_PauseVAGStream __attribute__((alias("FUN_0012ec70")));
