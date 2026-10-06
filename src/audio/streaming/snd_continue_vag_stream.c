#include "types.h"
extern s32 snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a,
                                        s32 b) __asm__("func_0012E6E0");
void snd_continue_vag_stream(s32 arg0) __asm__("FUN_0012eca0");

void snd_continue_vag_stream(s32 arg0) {
    u8 sp_slot[0x10];
    *(s32 *)(sp_slot + 0) = arg0;
    snd_send_iop_command_no_wait(46, 4, sp_slot, 0, 0);
}

extern __typeof__(snd_continue_vag_stream) func_0012ECA0 __attribute__((alias("FUN_0012eca0")));

/* Recovered original symbol name. */
extern __typeof__(snd_continue_vag_stream) snd_ContinueVAGStream
    __attribute__((alias("FUN_0012eca0")));
