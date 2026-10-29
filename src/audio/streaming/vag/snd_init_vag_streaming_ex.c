#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_send_iop_command_and_wait() __asm__("func_0012E548");
extern s32 snd_stream_safe_cd_sync() __asm__("func_0012EE08");
s32 snd_init_vag_streaming_ex(s32 arg0, s32 arg1, s32 arg2, s32 arg3) __asm__("FUN_0012eb20");

s32 snd_init_vag_streaming_ex(s32 configuration_word_0, s32 configuration_word_1, s32 configuration_word_2, s32 configuration_word_3) {
    s32 args[4];
    s32 result;

    if (snd_cd_stream_active == 1) {
        return 0;
    }
    if (snd_cd_callback_pending != 0) {
        while (snd_flush_sound_commands() != 0) {
        }
    }
    snd_stream_safe_cd_sync(0);
    args[0] = configuration_word_0;
    args[1] = configuration_word_1;
    args[2] = configuration_word_2;
    args[3] = configuration_word_3;
    result = snd_send_iop_command_and_wait(0x2A, 0x10, args);
    snd_cd_stream_active = result;
    return result;
}

extern __typeof__(snd_init_vag_streaming_ex) func_0012EB20 __attribute__((alias("FUN_0012eb20")));
