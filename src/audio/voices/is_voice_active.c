#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/data_labels.h"

struct VoiceState {
    u8  pad_0[0x74];
    u8  unk74;
    u8  pad_75[0x9];
    s16 unk7E;
};

s32 is_voice_active(s32 voice_index, s32 sound_id) __asm__("FUN_001eb740");

s32 is_voice_active(s32 voice_index, s32 sound_id) {
    struct VoiceState *voice;

    if (voice_index >= 0) {
        voice = (struct VoiceState *)(voice_index * 0x70 + ((u8 *)&voice_pool));
        if (voice->unk7E == sound_id + D_0015F5B4) {
            if ((u32)(voice->unk74 - 1) < 2) {
                return 1;
            }
        }
    }
    return 0;
}

extern __typeof__(is_voice_active) func_001EB740 __attribute__((alias("FUN_001eb740")));
