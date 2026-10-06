#include "types.h"
extern s32 D_0015EC8C;
extern s32 sceCdBreak(void);
extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");
s32 snd_stream_safe_cd_break(void) __asm__("FUN_0012eea8");

s32 snd_stream_safe_cd_break(void) {
    if (D_0015EC8C == 0) {
        return sceCdBreak();
    }
    snd_send_iop_command_no_wait(0x37, 0, 0, 0, 0);
    return 1;
}

extern __typeof__(snd_stream_safe_cd_break) func_0012EEA8 __attribute__((alias("FUN_0012eea8")));
