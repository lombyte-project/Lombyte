#include "types.h"
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
void snd_stream_safe_check_cd_idle(s32 arg0) __asm__("FUN_0012ed30");

void snd_stream_safe_check_cd_idle(s32 arg0) {
    u8 sp_slot[0x10];
    *(s32 *)sp_slot = arg0;
    snd_send_iop_command_and_wait(0x36, 4, sp_slot);
}

extern __typeof__(snd_stream_safe_check_cd_idle) func_0012ED30
    __attribute__((alias("FUN_0012ed30")));

/* Recovered original symbol name. */
extern __typeof__(snd_stream_safe_check_cd_idle) snd_StreamSafeCheckCDIdle
    __attribute__((alias("FUN_0012ed30")));
