#include "types.h"
#include "asm.h"
#include "rnc/audio/streaming/finish_audio_stream_read.h"
#include "types.h"

#include "rnc/audio/streaming/finish_audio_stream_read.h"
#include "types.h"

extern struct MusicStreamState D_001516D0;
extern s32 snd_stream_safe_cd_get_error() __asm__("func_0012EEF0");
void finish_audio_stream_read(s32 arg0) __asm__("FUN_00216950");

void finish_audio_stream_read(s32 arg0) {
    if (arg0 == 1) {
        D_001516D0.unk8 = 0;
        if (snd_stream_safe_cd_get_error() != 0) {
            D_001516D0.unk8 = 2;
        }
    }
}
