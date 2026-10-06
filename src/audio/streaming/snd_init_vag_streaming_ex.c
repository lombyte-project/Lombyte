#include "types.h"
extern s32 D_0015EC8C;
extern s32 D_0015ECC8;
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
extern s32 snd_stream_safe_cd_sync() __asm__("func_0012EE08");
s32 snd_init_vag_streaming_ex(s32 arg0, s32 arg1, s32 arg2, s32 arg3) __asm__("FUN_0012eb20");

s32 snd_init_vag_streaming_ex(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 args[4];
    s32 result;

    if (D_0015EC8C == 1) {
        return 0;
    }
    if (D_0015ECC8 != 0) {
        while (snd_flush_sound_commands() != 0) {
        }
    }
    snd_stream_safe_cd_sync(0);
    args[0] = arg0;
    args[1] = arg1;
    args[2] = arg2;
    args[3] = arg3;
    result = snd_send_iop_command_and_wait(0x2A, 0x10, args);
    D_0015EC8C = result;
    return result;
}

extern __typeof__(snd_init_vag_streaming_ex) func_0012EB20 __attribute__((alias("FUN_0012eb20")));
